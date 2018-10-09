$env:Path += ";C:\Users\Trevor\Stuff\mingw32\bin"
#SFML_INCLUDE="C:\Users\Trevor\Stuff\SFML-2.5.0-windows-gcc-7.3.0-mingw-32-bit\SFML-2.5.0\include"
#SFML_LIB="C:\Users\Trevor\Stuff\SFML-2.5.0-windows-gcc-7.3.0-mingw-32-bit\SFML-2.5.0\lib"
$SFML_INCLUDE="C:\Users\Trevor\Stuff\SFML-2.4.2-windows-gcc-6.1.0-mingw-32-bit\SFML-2.4.2\include"
$SFML_LIB="C:\Users\Trevor\Stuff\SFML-2.4.2-windows-gcc-6.1.0-mingw-32-bit\SFML-2.4.2\lib"
$SFE_INCLUDE="C:\Users\Trevor\Stuff\sfeMovie-2.0.0-Release-SFML-2.4.2\sfeMovie-2.0.0\include"
$SFE_LIB="C:\Users\Trevor\Stuff\sfeMovie-2.0.0-Release-SFML-2.4.2\sfeMovie-2.0.0\lib"
$WB_INCLUDE=".\source"
mkdir .\tmp
$output_files = ""
foreach ($source_file in get-childitem .\source *.cpp -recurse) {
  & gcc ("-O0 -g3 -Wall -c -fmessage-length=0 -I" + $SFML_INCLUDE + " -I" + $SFE_INCLUDE + " -I" + $WB_INCLUDE + " -o .\tmp\" + $source_file.basename + ".o " + $source_file.fullname + " -w").Split(" ")
  if ($source_file.basename -eq "main"){
	$output_files = ".\tmp\main.o " + $output_files
  } Else {
    $output_files += ".\tmp\" + $source_file.basename + ".o "
  }
}
& gcc ("-L" + $SFML_LIB + " -L" + $SFE_LIB + " -o .\release\WavyBoi.exe " + $output_files + "-static-libstdc++ -static-libgcc -lstdc++ -lws2_32 -lsfml-window -lsfml-graphics -lsfml-system -lsfml-network").Split(" ")
rmdir -r .\tmp