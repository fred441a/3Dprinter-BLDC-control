{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell{

  buildInputs = with pkgs; [
      doxygen
	  picotool
	  glibc_multi
	  newlib
	  tio
	  cmake
      gcc-arm-embedded
	  gnumake
	  ];

  shellHook = ''
  export PICO_SDK_PATH=$(pwd)/pico-sdk/
  export PICO_BOARD=pico_w
  alias monitor='tio --map INLCRNL,ODELBS /dev/ttyACM0'
  alias cmake=cmake -DPICO_STDIO_USB=1
  '';
}
