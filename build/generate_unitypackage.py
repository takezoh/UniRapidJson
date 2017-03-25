# coding: utf-8

# Copyright (c) 2017 Takehito Gondo
#
# UniRapidJson is released under the MIT License.
# http://opensource.org/licenses/mit-license.php

import uuid
import os
import time
import tempfile
import shutil
import tarfile
import gzip


PROJ_ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..')
DIST_PATH = os.path.join(PROJ_ROOT, 'dist', 'unity')
CS_SRC_PATH = os.path.join(PROJ_ROOT, 'src', 'runtime-support', 'unity', 'cs')
OUTPUT_PATH = os.path.join(PROJ_ROOT, 'dist', 'UniRapidJson.unitypackage')

ASSET_PREFIX_PATH = 'Assets/Standard Assets/UniRapidJson'

META_HEADER = """fileFormatVersion: 2
guid: {guid}
timeCreated: {time}
licenseType: Free"""

DEFAULT_IMPORTER = """
DefaultImporter:
  userData:
  assetBundleName:
  assetBundleVariant:
"""

PLUGIN_IMPORTER = """
PluginImporter:
  serializedVersion: 1
  iconMap: {{}}
  executionOrder: {{}}
  isPreloaded: 0
  platformData: {platform_data}
  userData:
  assetBundleName:
  assetBundleVariant:
"""

PLATFORM_DATA = {
    'Internal.Any': '''
    iOS:
      enabled: 1
      settings:
        CompileFlags:
        FrameworkDependencies:''',

    'Android.Any': '''
    Android:
      enabled: 1
      settings:
        CPU: AnyCPU''',

    'OSX.Any': '''
    OSXIntel:
      enabled: 1
      settings:
        CPU: AnyCPU
    OSXIntel64:
      enabled: 1
      settings:
        CPU: AnyCPU
    OSXUniversal:
      enabled: 1
      settings: {}
    Editor:
      enabled: 1
      settings:
        CPU: AnyCPU
        DefaultValueInitialized: true
        OS: OSX''',

    'Windows.x64': '''
    Win64:
      enabled: 1
      settings:
        CPU: AnyCPU
    Editor:
      enabled: 1
      settings:
        CPU: x86_64
        DefaultValueInitialized: true
        OS: Windows''',

    'Windows.x86': '''
    Win:
      enabled: 1
      settings:
        CPU: AnyCPU
    Editor:
      enabled: 1
      settings:
        CPU: x86
        DefaultValueInitialized: true
        OS: Windows''',

    'WinRT.ARM': '''
    WindowsStoreApps:
      enabled: 1
      settings:
        CPU: ARM
        DontProcess: False
        PlaceholderPath:
        SDK: AnySDK
        ScriptingBackend: AnyScriptingBackend''',

    'WinRT.x64': '''
    WindowsStoreApps:
      enabled: 1
      settings:
        CPU: X64
        DontProcess: False
        PlaceholderPath:
        SDK: AnySDK
        ScriptingBackend: AnyScriptingBackend''',

    'WinRT.x86': '''
    WindowsStoreApps:
      enabled: 1
      settings:
        CPU: X86
        DontProcess: False
        PlaceholderPath:
        SDK: AnySDK
        ScriptingBackend: AnyScriptingBackend''',
}


def get_guid(metapath):
    with open(metapath, 'r') as f:
        for l in f.readlines():
            if l.lstrip().startswith('guid:'):
                return l.split(':', 1)[1].strip()
    return None


def default_importer(platform, arch):
    return DEFAULT_IMPORTER


def plugin_importer(platform, arch):
    return PLUGIN_IMPORTER.format(
            platform_data=PLATFORM_DATA[platform + '.' + arch])


def generate_meta_file(platform, arch, asset, importer, isdir):
    meta_file = asset + '.meta'

    with open(meta_file, 'w') as f:
        header = META_HEADER.format(
            time=int(time.time()),
            guid=uuid.uuid4().hex)

        if isdir:
            header += "\nfolderAsset: yes"

        f.write(header + importer(platform, arch))


def generate_package(tmpdir):
    for root, dirs, files in os.walk(DIST_PATH):
        assets = [x for x in files if x.endswith('.meta')]
        folderassets = dirs
        module = root[len(DIST_PATH):]

        for metafile in assets:
            asset = metafile[:-5]
            guid = get_guid(os.path.join(root, metafile))

            try:
                module_dir = os.path.join(tmpdir, guid)
                os.makedirs(module_dir)
            except OSError:
                pass

            pathname_path = os.path.join(module_dir, 'pathname')
            assetmeta_path = os.path.join(module_dir, 'asset.meta')
            asset_path = os.path.join(module_dir, 'asset')

            with open(pathname_path, 'w') as f:
                f.write(ASSET_PREFIX_PATH + module + '/' + asset)

            shutil.copyfile(os.path.join(root, metafile), assetmeta_path)
            if not asset in folderassets:
                shutil.copyfile(os.path.join(root, asset), asset_path)

    #  with tarfile.open(OUTPUT_PATH, 'w:gz', format=tarfile.GNU_FORMAT, compresslevel=9) as archive:
    with tarfile.open(OUTPUT_PATH, 'w') as archive:
        archive.add(tmpdir, arcname='')


def main():
    for root, dirs, files in os.walk(CS_SRC_PATH):
        for script in [x for x in files if x.endswith('.cs') and not x.endswith('.meta')]:
            module = root[len(CS_SRC_PATH):]
            src = os.path.join(root, script)
            dst = os.path.join(DIST_PATH, module, script)
            shutil.copyfile(src, dst)
            generate_meta_file(None, None, dst, default_importer, isdir=False)

    for module in os.listdir(DIST_PATH):
        dir_path = os.path.join(DIST_PATH, module)
        if not os.path.isdir(dir_path):
            continue

        platform, arch = module.split('.', 1)
        importer = plugin_importer
        for root, dirs, files in os.walk(dir_path):
            for asset in [x for x in files if not x.endswith('.meta')]:
                generate_meta_file(platform, arch, os.path.join(root, asset), importer, isdir=False)
            for asset in dirs:
                generate_meta_file(platform, arch, os.path.join(root, asset), importer, isdir=True)
            importer = default_importer

    with tempfile.TemporaryDirectory() as tmpdir:
        generate_package(tmpdir)


if __name__ == '__main__':
    main()
