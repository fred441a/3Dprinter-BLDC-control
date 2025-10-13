{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell{
  packages = with pkgs;[
  stlink
  #stlink-gui
  gcc-arm-embedded
  openocd
  ];

}
