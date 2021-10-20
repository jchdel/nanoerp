#!/bin/sh
# a minimal ERP to manage our PV installation operations
# need: bash and bc
for i in bin/*;do $i help;echo;done|less
