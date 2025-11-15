{ pkgs ? import <nixpkgs> { } }:
pkgs.mkShell {
  packages = with pkgs; [
#    octaveFull.withPackages (opkgs: with opkgs; [ control signal ])
    libreoffice
  ];
}
