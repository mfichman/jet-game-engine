clsregex = /class\s*(\w*)\s*[:]{0,1}\s*(.*?)[{](.*?)[}][;]/mx
baseregex = /(public)\s*(\w*)\s*[,]{0,1}/
rregex = /([\w:<>]*)\s*(\w*)[(][)].*?;/
wregex = /([\w:<>]*)\s*(\w*)[(](.+?)[)].*?;/


#(\s*(\w*)\s*,\s*)+){0,1}
#([:](\s*\w*\s*[,]\s*)+){0,1}

hash = {}

class CppClass

    attr_accessor :accessors
    attr_accessor :mutators
    attr_accessor :parents 
    attr_accessor :name

    def initialize(name, hash)
        @name = name
        @hash = hash
        @accessors = []
        @mutators = []
        @parents = []
        @traversed = false
    end
    
    def resolve_members()
        if not @traversed 
            @parents.each do |parent|   
                pcls = @hash[parent] 
                if not pcls.nil?
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
            puts("    #{@name}::accessor(\"#{accessor}\", &#{@name}::#{accessor});")
        end
        @mutators.each do |mutator|
            puts("    #{@name}::mutator(\"#{mutator}\", &#{@name}::#{mutator});")
        end
    end
end


# Scan for class definitions

ARGV.each do |arg|
    text = File.new(arg).readlines.join
    while (clsmd = clsregex.match(text)) do
      
        cls = CppClass.new(clsmd[1], hash)
        hash[cls.name] = cls
        
        # Read all the parent classes
        text = clsmd[2]
        while (basemd = baseregex.match(text)) do

            cls.parents << basemd[2]        
            text = basemd.post_match
            
            #print("#{cls.name} : public #{basemd[2]}\n")
        end
        
        # Read all the accessor methods
        text = clsmd[3]
        while (mbrmd = rregex.match(text)) do
          #(::Ptr$)|
            if not /(Itr$)/ =~ mbrmd[1] then
                cls.accessors << mbrmd[2]
            end
            text = mbrmd.post_match
        
            #print("#{cls.name}::#{mbrmd[1]} [r]\n")
        end
        
        # Read all the mutator methods
        text = clsmd[3]
        while (mbrmd = wregex.match(text)) do
            if not /::Ptr$/ =~ mbrmd[1] then
                cls.mutators << mbrmd[2]
            end
            text = mbrmd.post_match
        
            #print("#{cls.name}::#{mbrmd[1]} [w]\n")
        end
        
        text = clsmd.post_match
    end
end

def dump(hash, io)
    hash.each_value do |cls|
        io.puts("#include <#{cls.name}.hpp>")
    end

    io.puts
    io.puts("void register()")
    io.puts("{")

    hash.each_value do |cls|
        cls.resolve_members
        cls.print_members(io)
    end

    io.puts("}")
end


dump(hash, $stdout)








