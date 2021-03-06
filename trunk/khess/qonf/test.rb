
require 'qmake'
require 'detectos'
require 'rexml/parsers/sax2parser'
require 'rexml/sax2listener'

require 'info'

module RQonf

class Test
	include REXML
	
	attr_reader :rules
	attr_reader :optional
	
	def initialize(rulesPath, qmake)
		@rules = rulesPath
		@qmake = qmake
		
		@optional = false
	end
	
	def run(config)
		parser = Parser.new
		parser.os = DetectOS::OS[DetectOS.whatOS].to_s.downcase
		
		return false if not parser.parse(@rules) or parser.name.empty?
		
		@optional = parser.optional
		
		Info.info << "Checking for " << parser.name << "... "
		
		dir = "#{File.dirname(@rules)}/#{parser.dir}"
		
		if $DEBUG
			Info.warn << "Running in " << dir << $endl
		end
		
		cwd = Dir.getwd
		if File.exists?(dir)
			if File.stat(dir).directory?
				Dir.chdir(dir)
				@qmake.run( "'INCLUDEPATH += #{parser.includes.join(" ")}' 'LIBS += #{parser.libs.join(" ")}'" ,true)
				if not @qmake.compile
					Dir.chdir(cwd)
					
					print "[ FAIL ]\n"
					
					# Enviar soluci�n
					solution = parser.solution
					
					Info.warn << "Seems like you are running " << parser.os << $endl
					Info.warn << "You will need to install " << solution[:package] << $endl
					Info.warn << "URL: " << solution[:url] << $endl
					Info.warn << solution[:comment] << $endl
					
					return false
				end
			else
				Dir.chdir(cwd)
				raise QonfException.new("'#{dir}' isn't a directory!")
				return false
			end
		else
			Dir.chdir(cwd)
			raise QonfException.new("'#{dir}' doesn't exists!")
			return false
		end
		
		Dir.chdir(cwd)
		
		parser.includes.each { |inc|
			config.addIncludePath(inc)
		}
		
		parser.libs.each { |lib|
			config.addLib(lib)
		}
		
		parser.defines.each { |define|
			config.addDefine(define)
		}

		parser.modules.each { |mod|
			config.addModule(mod)
		}
		
		print "[ OK ]\n"
		
		return true
	end
	
	def to_s
		@rules
	end
	
	private
	class Parser
		include REXML::SAX2Listener
		
		attr_reader :name
		attr_reader :dir
		attr_reader :defines
		attr_reader :includes
		attr_reader :libs
		attr_reader :modules
		attr_accessor :os
		attr_reader :solution
		attr_reader :optional
		
		def initialize
			@name = ""
			@current_tag = ""
			@section = ""
			@dir = ""
			@defines = []
			@includes = []
			@libs = []
			@modules = []
			@solution = {}
			@os = ""
			@optional = false
		end
		
		def start_document
		end
		
		def end_document
		end
		
		def start_element( uri, localname, qname, attributes)
			case qname
				when "qonf"
					@optional = (attributes["optional"] == "true")
				when "test"
					@section = qname
					@name = attributes["name"].to_s
				when "dir"
					if @section == "test"
						@dir = attributes["path"]
					end
				when "provide"
					@section = qname
				when "define"
					if @section  == "provide"
						@defines << attributes["value"]
					end
				when "include"
					if @section  == "provide"
						@includes << attributes["path"]
					end
				when "lib"
					if @section  == "provide"
						@libs << attributes["path"]
					end
				when "module"
					if @section == "provide"
						@modules << attributes["value"]
					end
				when "command"
					if @section == "provide"
						if @current_tag == "includes"
							IO.popen(attributes["value"]) { |c|
								output = c.readlines.join("").split(" ")
								
								output.each { |i|
									if i =~ /-I(.*)/
										@includes << $1.chomp
									elsif not i =~ /-.*/
										@includes << i
									end
								}
							}
						elsif @current_tag == "libs"
							IO.popen(attributes["value"]) { |c|
								output = c.readlines.join("").split(" ")
								
								output.each { |lib|
									if lib =~ /-l(.*)/
										@libs << lib
									elsif lib[0].chr != "-"
										@libs << lib
									end
								}
								@libs.concat(output)
							}
						end
					end
				when "solution"
					@section = "solution"
				when "windows"
					if @os == qname and @solution.size == 0
						fillSolution(attributes)
					end
				when "macosx"
					if @os == qname and @solution.size == 0
						fillSolution(attributes)
					end
				when "linux"
					if @os == qname and @solution.size == 0
						fillSolution(attributes)
					end
				when "distribution"
					if @os == attributes["name"].to_s.downcase and @solution.size == 0
						fillSolution(attributes)
					end
			end
			
			@current_tag = qname
		end
		
		def end_element( uri, localname, qname)
			case qname
				when ""
			end
			
		end
		
		def characters(text)
			text = text.strip
			if not text.empty?
			end
		end
		
		def parse(path)
			sax2 = REXML::Parsers::SAX2Parser.new( File.new( path ) )
			sax2.listen(self)
			
			begin
				sax2.parse
			rescue
				return false
			end
			
			return true
		end
		
		private
		def fillSolution(attributes)
			@solution[:package] = attributes["package"]
			@solution[:url] = attributes["url"]
			@solution[:comment] = attributes["comment"]
		end
	end
end

end #module

