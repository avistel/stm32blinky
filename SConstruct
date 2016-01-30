import os


OPTIMIZATION = '-O2'
#LDPATH = os.path.join(BASE, 'Projects/STM32F3-Discovery/Templates/TrueSTUDIO/STM32F3-Discovery/STM32F303VC_FLASH.ld')

env = Environment(ENV = os.environ, tools = ['mingw'])

env['AR'] = 'arm-none-eabi-ar'
env['AS'] = 'arm-none-eabi-as'
env['CC'] = 'arm-none-eabi-gcc'
env['CXX'] = 'arm-none-eabi-g++'
env['LINK'] = 'arm-none-eabi-gcc'
env['RANLIB'] = 'arm-none-eabi-ranlib'
env['OBJCOPY'] = 'arm-none-eabi-objcopy'
env['PROGSUFFIX'] = '.elf'

# include locations

env.Append(CPPPATH=
	[
		os.path.join(os.getcwd(),'thirdparty', 'cmsis', 'inc'),
		os.path.join(os.getcwd(),'thirdparty','cmsis','dev','ST','STM32F3xx','inc'),
		os.path.join(os.getcwd(),'thirdparty' , 'hal' , 'inc'),
		os.path.join(os.getcwd(),'thirdparty' , 'hal' , 'inc' , 'legacy')
	])


env.Append(CCFLAGS = [
	'-mcpu=cortex-m4',
	'-mthumb',
	OPTIMIZATION,
	'-fsigned-char',
	'-ffunction-sections',
	'-fdata-sections',
	'-std=c90',
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
	'STM32F3xx'
	])

Export('env')

print os.getcwd()
obj = SConscript('thirdparty/cmsis/SConscript')
obj = SConscript('thirdparty/hal/SConscript')
