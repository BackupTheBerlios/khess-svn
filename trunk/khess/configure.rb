#!/usr/bin/ruby

$: << "qonf"

require 'qonf/configure'
require 'qonf/info'

begin
	conf = RQonf::Configure.new(ARGV)

	if conf.hasArgument?("help") or conf.hasArgument?("h")
		puts <<_EOH_
Use: #{$0} [options]
	options:
		--help, -h:					Show this message
		--prefix=[prefix], -prefix [prefix]:		Sets prefix
		--with-debug:					Enable debug
_EOH_
		exit 0
	end
	
	conf.setTestDir("configure.tests")
	conf.verifyQtVersion("4.2.0")
	conf.createTests
	
	config = RQonf::Config.new
	
	conf.runTests(config)
	
	config.addModule("core")
	config.addModule("gui")
	config.addModule("svg")
	config.addModule("xml")
	config.addModule("network")
	
	config.addDefine("VERSION=0.9+svn")
	config.addDefine('VERSION_STR=\\\\\"0.9+svn\\\\\"')
	if not conf.hasArgument?("with-debug")
		config.addDefine("D_NODEBUG")
		config.addOption("silent")
	end
	
	unix = config.addScope("unix")
	unix.addVariable("MOC_DIR", ".moc")
	unix.addVariable("UI_DIR", ".ui")
	unix.addVariable("OBJECTS_DIR", ".obj")
	
	config.save("khess_config.pri")
	config.save("src/dlib/dlibconfig.pri")
	
	conf.createMakefiles
	
rescue => err
	Info.error << "Configure failed. error was: #{err.message}\n"
	if $DEBUG
		puts err.backtrace
	end
end

