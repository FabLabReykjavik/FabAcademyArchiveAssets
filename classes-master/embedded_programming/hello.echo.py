#
# hello.echo.py
#
# serial echo hello-world
#
# Neil Gershenfeld 2/20/23
#
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose, but must
# acknowledge this project. Copyright is retained and
# must be preserved. The work is provided as is; no
# warranty is provided, and users accept all liability.
#
import sys
string = ''
while True:
    char = sys.stdin.read(1)
    string += char
    sys.stdout.write("hello.echo.py: you typed "+string+'\r'+'\n');