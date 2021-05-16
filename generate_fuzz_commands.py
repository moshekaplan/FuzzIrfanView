import argparse
import sys

fuzz_targets = """\
ReadANI_W
ReadBLP_W
ReadCDR_W
ReadCLP_W
ReadCMP
ReadCasioCAM_W
ReadDCX_W
ReadDDS_W
ReadFITS_W
ReadG32
ReadG3_W
ReadGLCD_W
ReadMPO_W
ReadMosaic_W
ReadPCX_W
ReadPSP_W
ReadPVR_W
ReadRAS_W
ReadRAW_W
ReadSFW_W
ReadSGI_W
ReadSIF_W
ReadTTF_W
ReadWAD3
ReadWAL_W
ReadWBMP_W
ReadXBM_W
ReadXPM_W
Read_BIO_RAD_W
Read_BadPNG_W
Read_Recoil_W
Read_Utah_RLE
""".split()

base_fuzz_dir = r"C:\Users\moshe\Desktop\fuzz"
dynamorio_dir = r"C:\Users\moshe\Desktop\fuzz\dynamorio\build\bin32"
winafl_dir = r"C:\Users\moshe\Desktop\fuzz\winafl"
winafl_build_dir = winafl_dir + r"\build32\bin\Release"
harness_dir = r"C:\Users\moshe\Desktop\fuzz\Fuzz_Irfanview_Harness\Release"


def run_drrun(fuzz_target):
    cmd = f"""{dynamorio_dir}\\drrun.exe -c {winafl_build_dir}\\winafl.dll -debug -fuzz_iterations 10 -coverage_module "Fuzz_Formats_{fuzz_target}.exe" -coverage_module "FORMATS.DLL" -target_module "Fuzz_Formats_{fuzz_target}.exe" -target_method "call_{fuzz_target}" -nargs 1 -- {harness_dir}\\Fuzz_Formats_{fuzz_target}.exe "IMG_20210120_091018.jpg" """
    return cmd

def run_cmin(fuzz_target):
    cmd = f"""python {winafl_dir}\\winafl-cmin.py -w 3 --working-dir {winafl_build_dir} -i {base_fuzz_dir}\\corpus_combined -o {base_fuzz_dir}\\Fuzz_Formats_{fuzz_target}_cmin --crash-dir {base_fuzz_dir}\\Fuzz_Formats_{fuzz_target}_cmin_crashes --hang-dir {base_fuzz_dir}\\Fuzz_Formats_{fuzz_target}_cmin_hangs -t 4000 -D {dynamorio_dir} -covtype edge -coverage_module "Fuzz_Formats_{fuzz_target}.exe" -coverage_module "FORMATS.DLL" -target_module "Fuzz_Formats_{fuzz_target}.exe" -target_method "call_{fuzz_target}" -nargs 1 -- "{harness_dir}\\Fuzz_Formats_{fuzz_target}.exe" @@"""
    return cmd

def run_winafl(fuzz_target):
    cmd = f"""{winafl_build_dir}\\afl-fuzz.exe  -w {winafl_build_dir}\winafl.dll -i {base_fuzz_dir}\\Fuzz_Formats_{fuzz_target}_cmin -o {base_fuzz_dir}\\Fuzz_Formats_{fuzz_target}_winafl_output -t 1000+ -D {dynamorio_dir} -- -coverage_module "Fuzz_Formats_{fuzz_target}.exe" -coverage_module "FORMATS.DLL" -target_module "Fuzz_Formats_{fuzz_target}.exe" -target_method "call_{fuzz_target}" -nargs 1 -- "{harness_dir}\\Fuzz_Formats_{fuzz_target}.exe" @@ """
    return cmd

if len(sys.argv) != 2:
    print (f"Error! 1st argument must be an exported function! Options include {fuzz_targets}")
    sys.exit(1)

fuzz_target = sys.argv[1]
if fuzz_target not in fuzz_targets:
    print (f"Error! Unknown target of '{sys.argv[1]}'")
    sys.exit(1)

print ("# drrun command:")
print (run_drrun(fuzz_target))
print ()

print ("# cmin command:")
print (run_cmin(fuzz_target))
print ()

print ("# afl-fuzz command:")
print (run_winafl(fuzz_target))
print ()