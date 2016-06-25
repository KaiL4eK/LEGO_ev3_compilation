#!/usr/bin/env python
import xml.etree.cElementTree as ET
from xml.dom import minidom

compilerId = 'ProjectExplorer.ToolChain.Gcc:{6e186500-fa66-410d-a6f3-ff2749a3a964}'
mainHeaders = []
mainHeaders.append('<?xml version="1.0" encoding="UTF-8"?>\n')
mainHeaders.append('<!DOCTYPE QtCreatorToolChains>\n')

def prettify(elem):
    """Return a pretty-printed XML string for the Element."""
    rough_string = ET.tostring(elem, 'utf-8')
    reparsed = minidom.parseString(rough_string)
    return reparsed.toprettyxml(indent='\t')

def indent(elem, level=0):
    i = "\n" + level*"  "
    if len(elem):
        if not elem.text or not elem.text.strip():
            elem.text = i + "  "
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
        for elem in elem:
            indent(elem, level+1)
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
    else:
        if level and (not elem.tail or not elem.tail.strip()):
            elem.tail = i

def addChild(root, name, key=None, type=None, text=None):
    child = ET.SubElement( root, name )
    if key is not None:
        child.set('key', key)
    if type is not None:
        child.set('type', type)
    if text is not None:
        child.text = text
    return child

def writeNewDevice():
    print "Write new device"
    tree = ET.ElementTree(file='filename.xml')
    root = tree.getroot()
    deviceList = None
    for valuelist in root.iter('valuelist'):
        if valuelist.get('key') == 'DeviceList':
            deviceList = valuelist

    if deviceList is None:
        return None

    new_device = ET.Element('valuemap')
    new_device.set( 'type', 'QVariantMap' )

    addChild( root=new_device, name='value', 
        key='Authentication',
        type='int',
        text = '1' )

    


def writeNewCompiler():
    count = 0
    tree = ET.ElementTree(file='toolchains.xml')
    root = tree.getroot()

    for dataNode in root:
        variableNode = dataNode.find('variable')
        if variableNode.text == 'ToolChain.Count':
            valueNode = dataNode.find('value')
            count = int(valueNode.text)
            valueNode.text = str(count+1)

    new_compiler = ET.Element('data')

    variable = addChild( root=new_compiler, name='variable', 
        text='ToolChain.{}'.format(count) )

    valuemap = addChild( root=new_compiler, name='valuemap', 
        type='QVariantMap' )

    addChild( root=valuemap, name='value', 
        key='ProjectExplorer.GccToolChain.OriginalTargetTriple',
        type='QString',
        text = 'arm-linux-gnueabi' )

    addChild( root=valuemap, name='value', 
        key='ProjectExplorer.GccToolChain.Path',
        type='QString',
        text = '/usr/bin/arm-linux-gnueabi-gcc' )

    addChild( root=valuemap, name='value', 
        key='ProjectExplorer.GccToolChain.TargetAbi',
        type='QString',
        text = 'arm-linux-generic-elf-32bit' )

    addChild( root=valuemap, name='value', 
        key='ProjectExplorer.ToolChain.Autodetect',
        type='bool',
        text = 'false' )

    addChild( root=valuemap, name='value', 
        key='ProjectExplorer.ToolChain.DisplayName',
        type='QString',
        text = 'EV3 Compiler' )

    addChild( root=valuemap, name='value', 
        key='ProjectExplorer.ToolChain.Id',
        type='QString',
        text = compilerId )

    root.append(new_compiler)
    indent(root)

    headers = mainHeaders
    headers.append(ET.tostring(root))

    # tree.write('new.xml')
    with open('new.xml', 'w') as output:
        for item in headers:
            output.write(item)

def main():
    # writeNewCompiler()
    writeNewDevice()

if __name__ == '__main__':
    main()

