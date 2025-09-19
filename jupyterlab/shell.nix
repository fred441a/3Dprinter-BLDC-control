{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell{
  packages = with pkgs; [
  pandoc
  jupyter
  texliveTeTeX
  texliveFull
  ];
  shellHook = "jupyter-lab";
}
