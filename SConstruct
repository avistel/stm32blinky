import os
import sys

from SCons.Tool import gcc

DefaultEnvironment(tools=[])

gcc.detect_version = lambda env, cc: None

env = Environment(
		AR = 'arm-none-eabi-ar',
		AS = 'arm-none-eabi-as',
		CC = 'arm-none-eabi-gcc',
		CXX = 'arm-none-eabi-g++',
		LINK = 'arm-none-eabi-gcc',
		RANLIB = 'arm-none-eabi-ranlib',
		OBJCOPY = 'arm-none-eabi-objcopy',
		PROGSUFFIX = '.elf',
		tools = ['mingw'],
		ENV = {'PATH' : os.environ['PATH'], 'CYGWIN' : 'nodosfilewarning'}
)

# include locations

env.Append(CPPPATH = [
	'#/' + os.path.join('thirdparty','cmsis','inc'),
	'#/' + os.path.join('thirdparty','cmsis','dev','ST','STM32F3xx','inc'),
	'#/' + os.path.join('thirdparty','hal','inc'),
	'#/' + os.path.join('thirdparty','hal','inc','legacy')]
)

env.Append(CCFLAGS = [
	'-mcpu=cortex-m4',
	'-mthumb',
	'-O2',
	'-fsigned-char',
	'-ffunction-sections',
	'-fdata-sections',
	'-std=c99',
	'-fmessage-length=0',
	'-mthumb-interwork'
	])

# env.Append(LINKFLAGS = [
# '-ffunction-sections',
#     '-fdata-sections',
#     '-T' + LDPATH,
#     '-Xlinker',
#     '--gc-sections',
#     '--specs=nano.specs',
# 	])
env.Append(CPPDEFINES = [
	'STM32F3xx',
	'STM32F303xC'
	])

#print BASE
#print os.path.join(BASE, r'thirdparty/cmsis')
#print Dir(BASE).path
print '#' + os.path.join(os.getcwd(),'thirdparty','cmsis','inc')
print env['CPPPATH']
hal_o = SConscript('thirdparty/hal/SConscript', exports = 'env', duplicate=0)
cmsis_o = SConscript('thirdparty/cmsis/SConscript', exports = 'env', duplicate=0)
hal_a = env.Library(hal_o)
cmsis_a = env.Library(cmsis_o)
#obj = env.Object('stm32303c-eval-blinky.c')
