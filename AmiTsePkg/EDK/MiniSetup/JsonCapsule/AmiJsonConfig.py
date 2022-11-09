import sys
import subprocess
import struct
import os
import json

#This function is created to provide the help usage of the script
def usage_AmiJsonConfig():
    print ("\nusage: AmiJsonConfig.py [-h]")
    print ("To prodvide specific input file name:\n \t AmiJsonConfig.py  [-i] <input file name.(json)>")
    print ("\nTo prodvide specific output file name:\n \t AmiJsonConfig.py  [-o] <output file name.(json)>")
    print ("\nTo prodvide specific capsule Id:\n \t AmiJsonConfig.py  [-c] <CapsuleID>")
    print ("\nTo create a signed capsule with password  :\n \t AmiJsonConfig.py  [-sign] <password>")
    print ("\nTo get a report of specific capsule file:\n \t AmiJsonConfig.py  [-r] <capsule file name.(cap)>")
    print ("\n\nNote:\n1. The expression enclosed by <> means it is a mandatory field")
    print ("2. The expression enclosed by [] means it is a optional field")
    print ("3. If you provide any option argument that is -i, -o, -c, -sign then it is must to provide the next mandatory argument")
    print ("4. If Input file name is provided then the file extension has to be (.json)\n5.If Output file name is provided then the file extension has to be (.cap)\n6.CapsuleID should be numeric")
    print ("7. If Input file name is not provided then it is must to keep a file with name (AmiJsonCapsule.json) in the root")
    print ("8. If Output file name is not provided then the script will generate a capsule file with name (AmiJsonCapsule.cap)")
    sys.exit()

#This function is created to provide the report of any existing capsule file
def Report_Capsule():
    i = 0;
    try:
        f = open(ReportJson, "rb")
    except:
        print ("No such Capsule file is available to provide the report")
        sys.exit()
        
    file =open("ResultJson.json","wb")
    try:
        byte = f.read(1)
        while byte != b"":
            i = i+1
            # Do stuff with byte.
            byte = f.read(1)
            if i==36:
                hex_val_string = format(ord(byte), "x")

            if i==37: #offset 37 & 36 holds the hex value of actual Json Payload length
                hex_val_string1 = format(ord(byte), "x")
                finalHex = hex_val_string1 + hex_val_string
                Payload = int(finalHex, 16)
                Payload = Payload + 39

            if i>=40 and i<Payload:
                file.write(byte)

        file.close()
        
        print ("Capsule result is dumped into the file:- ResultJson.json")
        sys.exit()
        
    finally:
        f.close()

#This function is created to provide the file size
def file_size(fname):
    statinfo = os.stat(fname)
    return statinfo.st_size

arguments = len(sys.argv) - 1

#Defining the default file names
position = 1
signedimage = 0
inputfile = "AmiJsonCapsule.json"
outputfile = "AmiJsonCapsule.cap"
password = ""
zero = '\0'
CapsuleID = 1
FileOverride = 0
ReportJson = ""
ReportCapsule = 0

#Deleting the Exisiting Files
if os.path.exists("AmiJsonCapsule.pvk"):
    os.remove("AmiJsonCapsule.pvk")

if os.path.exists("AmiJsonCapsulecert.cer"):
    os.remove("AmiJsonCapsulecert.cer")
    
if os.path.exists("AmiJsonCapsulecert.pfx"):
    os.remove("AmiJsonCapsulecert.pfx")
    
if os.path.exists("JSONData.bin"):
    os.remove("JSONData.bin")

if os.path.exists("JSONData.bin.p7"):
    os.remove("JSONData.bin.p7")

if os.path.exists("TempJson.cap"):
    os.remove("TempJson.cap")
     
#Processing CommandLine arguments
while(arguments >= position):
    if sys.argv[position] == '-h':
        usage_AmiJsonConfig()
        sys.exit()
        
    if sys.argv[position] == '-i':
       try: 
           inputfile = sys.argv[position + 1]
       except:
           print("Please Provide the input file name")
           sys.exit()
           
    if sys.argv[position] == '-o':
       try:
           outputfile = sys.argv[position + 1]
       except:
           print("Please Provide the output file name")
           sys.exit()
           
    if sys.argv[position] == '-c':
        try:
           CapsuleID = int(sys.argv[position + 1])
        except:   
           print("Please Provide the Capsule Id")
           sys.exit()

    if sys.argv[position] == '-r':
        ReportCapsule = 1
        try:
           ReportJson = sys.argv[position + 1]
        except:   
           print("Please Provide the Capsule file to get the report")
           sys.exit()
                   
    if sys.argv[position] == '-sign':
       signedimage = 1
       try:
        password = sys.argv[position + 1]
       except IndexError:
        print ("Please Provide the Password")
        sys.exit()
       
    position = position + 1  

#if it is capsule report request
if ReportCapsule == 1:
    Report_Capsule()

#checking the existance of output capsule file    
if os.path.exists(outputfile):
    FileOverride = 1

#Checking the input output file formats   
if not(inputfile.endswith('.json')):
    print ("Wrong Input file format")
    sys.exit()
    
if not(outputfile.endswith('.cap')):
    print ("Wrong output file format")
    sys.exit()
       
#Reading the Json File
try:
    Jsonfile = open(inputfile, "rb")
except:
    print ("No such Input file is available")
    sys.exit()

buffer = Jsonfile.read()

#checking the Json data format
try:
    json.loads(buffer)
except:
    print ("Invalid input Json file format\n Please provide the Json file with correct format")
    sys.exit()

#parsing the Json data to get the payload length    
PayloadLength = file_size(inputfile)
Jsonfile.close()

#Reading the FwCapsuleHdr file
if signedimage == 1: 
    ffsfile = open("FwCapsuleHdr.ffs", "rb")
    ffsbuffer = ffsfile.read()
    ffsbufferlength = file_size("FwCapsuleHdr.ffs")
    ffsfile.close()
    CapsuleImageSize = PayloadLength + 1 + ffsbufferlength + 1 + 40

else:
   CapsuleImageSize = PayloadLength + 1 + 40

#Filling the Capsule Header structure
var1 = struct.pack('lHH8B3I',0x67d6f4cd,0xd6b8,0x4573,0xbf,0x4a,0xde,0x5e,0x25,0x2d,0x61,0xae,28,0x70000,CapsuleImageSize)

#Filling the Json Capsule Header structure
var2 = struct.pack('III',CapsuleID,1,PayloadLength + 1)

#creating the TempJson.cap file when Capsule is Signed
if signedimage == 1:
   file2 =open("TempJson.cap","wb")
   file2.write(var1)
   file2.write(var2)
   file2.write(buffer)
   file2.write(zero.encode())
   file2.write(ffsbuffer)
   file2.write(zero.encode())
   file2.close()
   
#creating the JSONData.bin file
   file3 =open("JSONData.bin","wb")
   file3.write(var2)
   file3.write(buffer)
   file3.write(zero.encode())
   file3.close()

#defining the commands path for signing the capsule image
   path1 = '"C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Bin/makecert.exe" -sv ./AmiJsonCapsule.pvk -n "CN=AMI JSON CAPSULE CERTIFICATE" ./AmiJsonCapsulecert.cer -r -a sha256 /len 2048'
   path2 = '"C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Bin/pvk2pfx.exe" -pvk ./AmiJsonCapsule.pvk -spc ./AmiJsonCapsulecert.cer -pi ' + password + ' -pfx ./AmiJsonCapsulecert.pfx'
   path3 = 'signtool.exe sign /fd sha256 /p7 ./ /p7co 1.2.840.113549.1.7.1 /p7ce DetachedSignedData /f AmiJsonCapsulecert.pfx /p ' + password + ' JSONData.bin'
   path4 = 'cryptocon -y -c2 -s -x JSONData.bin.p7 -f TempJson.cap -o ' + outputfile

#Executing the commands to sign the capsule and creating the final Capsule.cap file   
   subprocess.call(path1, shell=True)
   subprocess.call(path2, shell=True)
   subprocess.call(path3, shell=True)
   subprocess.call(path4, shell=True) 

#Capsule is not Signed
else:
    file4 =open(outputfile,"wb")
    file4.write(var1)
    file4.write(var2)
    file4.write(buffer)
    file4.write(zero.encode())
    file4.close()
    if FileOverride == 1:
        print ("Capsule file is generated successfully by overriding " + outputfile ) 
    else:
        print ("Capsule file is generated successfully as " + outputfile)
        
