#######################################################################
# Top-level SConstruct
#
# For example, invoke scons as 
#
#   scons debug=1 dri=0 machine=x86
#
# to set configuration variables. Or you can write those options to a file
# named config.py:
#
#   # config.py
#   debug=1
#   dri=0
#   machine='x86'
# 
# Invoke
#
#   scons -h
#
# to get the full list of options. See scons manpage for more info.
#  

import os
import os.path
import sys
import SCons.Util

import common

#######################################################################
# Configuration options

opts = Variables('config.py')
common.AddOptions(opts)

opts.Add(EnumVariable('MSVS_VERSION', 'MS Visual C++ version', None, allowed_values=('7.1', '8.0', '9.0')))

env = Environment(
	options = opts,
	tools = ['gallium'],
	toolpath = ['#scons'],	
	ENV = os.environ,
)

if os.environ.has_key('CC'):
	env['CC'] = os.environ['CC']
if os.environ.has_key('CFLAGS'):
	env['CCFLAGS'] += SCons.Util.CLVar(os.environ['CFLAGS'])
if os.environ.has_key('CXX'):
	env['CXX'] = os.environ['CXX']
if os.environ.has_key('CXXFLAGS'):
	env['CXXFLAGS'] += SCons.Util.CLVar(os.environ['CXXFLAGS'])
if os.environ.has_key('LDFLAGS'):
	env['LINKFLAGS'] += SCons.Util.CLVar(os.environ['LDFLAGS'])

Help(opts.GenerateHelpText(env))

# replicate options values in local variables
debug = env['debug']
machine = env['machine']
platform = env['platform']

# derived options
x86 = machine == 'x86'
ppc = machine == 'ppc'
gcc = platform in ('linux', 'freebsd', 'darwin', 'embedded')
msvc = platform in ('windows', 'winddk')

Export([
	'debug', 
	'x86', 
	'ppc', 
	'platform',
	'gcc',
	'msvc',
])


#######################################################################
# Environment setup

# Includes
env.Prepend(CPPPATH = [
	'#/include',
])

if env['msvc']:
    env.Append(CPPPATH = ['#include/c99'])

# Embedded
if platform == 'embedded':
	env.Append(CPPDEFINES = [
		'_POSIX_SOURCE',
		('_POSIX_C_SOURCE', '199309L'), 
		'_SVID_SOURCE',
		'_BSD_SOURCE', 
		'_GNU_SOURCE',
		
		'PTHREADS',
	])
	env.Append(LIBS = [
		'm',
		'pthread',
		'dl',
	])

# Posix
if platform in ('posix', 'linux', 'freebsd', 'darwin'):
	env.Append(CPPDEFINES = [
		'_POSIX_SOURCE',
		('_POSIX_C_SOURCE', '199309L'), 
		'_SVID_SOURCE',
		'_BSD_SOURCE', 
		'_GNU_SOURCE',
		
		'PTHREADS',
		'HAVE_POSIX_MEMALIGN',
	])
	if platform == 'darwin':
		env.Append(CPPDEFINES = ['_DARWIN_C_SOURCE'])
	env.Append(LIBS = [
		'm',
		'pthread',
		'dl',
	])

# for debugging
#print env.Dump()

Export('env')


#######################################################################
# Invoke SConscripts

# TODO: Build several variants at the same time?
# http://www.scons.org/wiki/SimultaneousVariantBuilds

SConscript(
	'src/SConscript',
	variant_dir = env['build'],
	duplicate = 0 # http://www.scons.org/doc/0.97/HTML/scons-user/x2261.html
)

