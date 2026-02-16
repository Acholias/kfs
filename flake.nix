{
  inputs = {
    flake-utils.url = "github:numtide/flake-utils";
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.11";
  };
  outputs = inputs:
    inputs.flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import inputs.nixpkgs { inherit system; };
      in {
        devShells.default = pkgs.mkShell {
          shell = pkgs.zsh;
          buildInputs = [
            pkgs.pkgsCross.i686-embedded.buildPackages.gcc
            pkgs.zsh
            pkgs.mtools
            pkgs.qemu
            pkgs.grub2
            pkgs.wget
            pkgs.unzip
          ];
          shellHook = ''
            exec zsh
          '';
        };
      });
}
