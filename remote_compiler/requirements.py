#!/usr/bin/env python

#######################
# Install dependencies
#######################
import pip
import os

out_folder = "out"
proto_name = "compiler"
pip.main(["install", "-r", "requirements.txt"])

#######################
# Generate protos
#######################
import grpc_tools.protoc as protoc

if not os.path.exists(out_folder):
    os.mkdir(out_folder)

protoc.main(["-Iprotos.", "--python_out=.", "--grpc_python_out=.", f"protos/{proto_name}.proto"])

newlines = []
with open(f"protos/{proto_name}_pb2_grpc.py", 'r') as file:
    for line in file.readlines():
        if f"from protos import {proto_name}_pb2" in line:
            newlines.append(f"import out.{proto_name}_pb2 as protos_dot_{proto_name}__pb2")
            continue
        newlines.append(line)


with open(f"protos/{proto_name}_pb2_grpc.py", 'w') as file:
    file.writelines(newlines)

os.replace(f"protos/{proto_name}_pb2_grpc.py", f"{out_folder}/{proto_name}_pb2_grpc.py")
os.replace(f"protos/{proto_name}_pb2.py", f"{out_folder}/{proto_name}_pb2.py")
