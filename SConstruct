
import os

common_env = Environment()
common_env.Append(CPP_DEFINES={'VERSION': 1})

release_env = common_env.Clone()
release_env.Append(CPP_DEFINES=['RELEASE'])
release_env.Append(CXXFLAGS=['-std=c++14'])
release_env.VariantDir('build/release', 'src')

debug_env = common_env.Clone()
debug_env.Append(CPP_DEFINES=['DEBUG'])
debug_env.VariantDir('build/debug', 'src')

for root, directories, filenames in os.walk('./src'):
    if 'SConscript' in filenames:
        prefix_path = os.path.relpath(root, './src')
        print prefix_path
        print os.path.normpath(os.path.join('build/release', prefix_path, 'SConscript'))
        release_env.SConscript(os.path.join('build/release', prefix_path, 'SConscript'), {'env': release_env})
