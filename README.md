# sds_gen
Simple digital signature generator based on standart GOST 34.10-2018  
This program is used to generate 256bit digital signature to the end of the file

[DS verifier](https://github.com/mitchon/sds_ver_auto)  (automatic one)  
[DS verifier](https://github.com/mitchon/sds_ver) (non-automatic one)  

Tested in Ubuntu 20.04 LTS

## Installation
<code>make</code>  
<code>make install</code>

## Uninstall
<code>make uninstall</code>

## Start
To start a program use <code>sds_gen</code>. This executable is stored in `/usr/local/bin`.  
To get help type <code>sds_gen -h</code>

## Configuration files
Configs are in `/usr/local/etc/sds`  
ds_params contains digital signature parameters  
public_accounts contains user login and his public keys  
accounts contains user login and his both private and public keys  
