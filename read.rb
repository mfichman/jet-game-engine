
require 'Set'

#(\s*(\w*)\s*,\s*)+){0,1}
#([:](\s*\w*\s*[,]\s*)+){0,1}

hash = {}

class CppClass

    attr_accessor :accessors
    attr_accessor :mutators
    attr_accessor :parents 
    attr_accessor :name
    attr_accessor :namespace

    def initialize(name, hash)
        @name = name
        @hash = hash
        @accessors = []
        @mutators = []
        @parents = []
        @traversed = false
        @namespace = []
    end
    
    def valid_interface()
        return @parents.member?("Interface")
    end
    
    def include_file()
        s = nil
        @namespace.each do |n|
            s = s.nil? ? n : "#{s}/#{n}"
        end
        s = s.nil? ? @name : "#{s}/#{@name}.hpp"
        return s
    
    end
    
    def qualified_name()
        s = nil
        @namespace.each do |n|
            s = s.nil? ? n : "#{s}::#{n}"
        end
        s = s.nil? ? @name : "#{s}::#{@name}"
        return s
    end
    
    def resolve_members()
        if not @traversed 
            @parents.each do |parent|   
                pcls = @hash[parent] 
                if not pcls.nil? and not pcls.name == "Interface"
                    pair = pcls.resolve_members
                    @accessors += pair[1]
                    @mutators += pair[0]
                end
            end
        end

        @traversed = true
        return [@mutators, @accessors]
    end
    
    def print_members(io)
        @accessors.each do |accessor|
            io.puts("    c->accessor(\"#{accessor}\", &#{qualified_name}::#{accessor});")
        end
        @mutators.each do |mutator|
            io.puts("    c->mutator(\"#{mutator}\", &#{qualified_name}::#{mutator});")
        end
    end
end

class CppFile

    NSPREGEX = /namespace\s*(\w*)\s*[{](.*)[}]/mx
    CLSREGEX = /class\s*(\w*)\s*[:]{0,1}\s*([\w\s,]*?)[{](.*?)[}][;]/mx
    BASEREGEX = /(public)\s*(\w*)\s*[,]{0,1}/
    RREGEX = /([\w:<>]*)\s*(\w*)[(][)].*?;/
    WREGEX = /([\w:<>]*)\s*(\w*)[(](.+?)[)].*?;/
    
    attr_accessor :namespaces

    def initialize(file, hash)
        @lines = File.new(file).readlines.join
        @hash = hash
        @namespaces = Set.new
        @namespace_stack = []
    end
    
    def read()
        readNamespaces(@lines)
    end
    
    def readNamespaces(text)
        nsp = nil
        while (nspmd = NSPREGEX.match(text)) do
        
            @namespace_stack.push(nspmd[1])
            if (nsp) then 
                nsp = nsp + "::" + nspmd[1]
            else
                nsp = nspmd[1]
            end
            @namespaces.add(nsp)
            text = nspmd[2]
        end
        
        readClasses(text)
    end
    
    def readClasses(text)
        while(clsmd = CLSREGEX.match(text)) do
            @cls = CppClass.new(clsmd[1], @hash)
            @cls.namespace += @namespace_stack
            
            @hash[@cls.name] = @cls
            readBaseClasses(clsmd[2])
            readAccessors(clsmd[3])
            readMutators(clsmd[3])
            text = clsmd.post_match
        end
    end
    
    def readBaseClasses(text)
                
        # Read all the parent classes
        while (basemd = BASEREGEX.match(text)) do
            @cls.parents << basemd[2]        
            text = basemd.post_match
            
            #print("#{cls.name} : public #{basemd[2]}\n")
        end
    end
    
    def readAccessors(text)
        # Read all the accessor methods
        while (mbrmd = RREGEX.match(text)) do
            #(::Ptr$)|
            if not /(Itr$)/ =~ mbrmd[1] and not /make/ =~ mbrmd[2] and not mbrmd[2].empty? then
                @cls.accessors << mbrmd[2]
            end
            text = mbrmd.post_match
         
            #print("#{cls.name}::#{mbrmd[1]} [r]\n")
        end
    end
    
    def readMutators(text)
        # Read all the mutator methods
        while (mbrmd = WREGEX.match(text)) do
            if not /::Ptr$/ =~ mbrmd[1] and not mbrmd[2].empty? then
                @cls.mutators << mbrmd[2]
            end
            text = mbrmd.post_match
        
            #print("#{cls.name}::#{mbrmd[1]} [w]\n")
        end
    end
end


# Scan for class definitions

namespaces = Set.new
dir = ARGV[0]



def read(hash, namespaces, dir)

    skip = ["Types.hpp", "Interface.hpp", "Iterator.hpp", "Transformer.hpp", "Registry.hpp"]

    Dir.foreach(dir) do |str|  
        entry = "#{dir}\\#{str}"
        if (/.*[.]hpp$/ =~ entry and not skip.member?(str)) then
            file = CppFile.new(entry, hash)
            file.read
            namespace = namespaces.union(file.namespaces)
        elsif (File.directory?(entry) and str != "." and str != "..") then
            read(hash, namespaces, entry)
        end
    end
end


def dump(hash, namespaces, io)
    io.puts("/* WARNING this file is auto-generated.  Do not modify */")
	io.puts("#include <Jet/Registry.hpp>")
    hash.each_value do |cls|
        if (cls.valid_interface) then
            io.puts("#include <#{cls.include_file}>")
        end
    end

	io.puts
	
	io.puts("using namespace Jet;")
	
    io.puts 
    io.puts("Registry::Registry() {")
    io.puts("    Class::Ptr c;");

    hash.each_value do |cls|
        cls.resolve_members
        
        if (cls.valid_interface) then
            io.puts("    // class #{cls.qualified_name}")
            io.puts("    c = type<#{cls.qualified_name}>();");
            cls.print_members(io)
            io.puts
        end
    end

    io.puts("}")
end

read(hash, namespaces, dir)

File.open("Source/Registry.cpp", "w") do |io|
	dump(hash, namespaces, io)
end








