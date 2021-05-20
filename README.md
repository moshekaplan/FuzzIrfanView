# FuzzIrfanView

Here is the accompany repository for my blog post, [Fuzzing IrfanView with WinAFL](https://www.linkedin.com/pulse/fuzzing-irfanview-win-afl-moshe-kaplan/).
It contains the following:

1. The scripts used to download files from bug trackers, located in `scraping_scripts`
2. The Visual Studio solution with the fuzzing harnesses for all of Formats.dll's exported functions, located in `Fuzz_Irfanview_Harness`
3. The corpora for each exported function, located in `fuzzing_corpora_cmin`
4. IrfanView version 4.57, located in `iview457`
5. Two helper scripts, `append_extension.py` and `replace_header.py`, for preparing samples when testing crashes in IrfanView
6. A final helper script, `generate_fuzz_commands.py`, for generating `drrun`, `winafl-cmin.py`, and `afl-fuzz.exe` commands

# Notes on replicating fuzzing crashes in IrfanView
All of the functions in Formats.dll trust that the file provided is the relevant format. For many formats, Irfanview relies upon [magic numbers](https://en.wikipedia.org/wiki/Magic_number_(programming)#Format_indicators) to identify the file types. However, for some, it relies upon the file's extension.

For the files in formats.dll, here are notes on exported functions that required closer examination:
* ReadCMP - Not officially supported in IrfanView
* ReadG32 - Not officially supported in IrfanView
* ReadG3_W - Requires file extension `.g3`
* ReadGLCD_W - I could not determine how to trigger this function from IrfanView
* ReadMosaic_W - Requires file extension `.mos`. Requires patching to avoid the Windows UI prompt.
* ReadMPO_W - Requires file extension `.mpo`
* ReadSIF_W - Content must begin with bytes `SIF`
* ReadSFW_W - Content must begin with bytes `SFW94A` (might only need to be `SFW`)
* ReadXBM_W - Requires file extension `.xbm`
* ReadWBMP_W - Requires file extension `.wbmp`
* ReadXPM_W - Requires file extension `.xpm`

# Potential future work

Here are a few ideas for potential future work:
* Fuzz IrfanView's other plugins
* Fuzz the formats parsed within IrfanView itself
* Fuzz with other fuzzers besides winafl
