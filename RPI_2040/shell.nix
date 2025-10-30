{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell{

  buildInputs = with pkgs; [
      doxygen
	  arduino-cli
	  arduino-ide
	  usbutils
	  udev
	  glibc_multi
	  tio
	  ];

  shellHook = ''
  export ARDUINO_CONFIG_FILE=$(pwd)/arduino-cli.yml
  export LD_LIBRARY_PATH=${pkgs.udev}/lib:$LD_LIBRARY_PATH
  alias upload='arduino-cli upload'
  alias compile='arduino-cli compile --fqbn rp2040:rp2040:rpipico:os=freertos'
  alias monitor='tio --map INLCRNL,ODELBS /dev/ttyACM0'
  '';
}
