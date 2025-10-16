{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell{

  buildInputs = with pkgs; [
	  arduino-ide
	  arduino-cli
	  usbutils
	  udev
	  ];


  shellHook = ''
  export ARDUINO_CONFIG_FILE=$(pwd)/arduino-cli.yml
  export LD_LIBRARY_PATH=${pkgs.udev}/lib:$LD_LIBRARY_PATH
  alias upload='arduino-cli upload'
  alias compile='arduino-cli compile'
  arduino-cli lib install freertos
  '';
}
