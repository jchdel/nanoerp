_is_number(){
	#re='^[0-9]+$'
	re='^[0-9]+([.][0-9]+)?$'
	[[ $1 =~ $re ]] && return 0
	echo "$1 is not a valid number (positive dot separated decimal)." >&2 
	_exit 1
}

_exit(){
	cd $PWD
	exit $1
}
########################################################################
ME=$(basename $0)
PWD=$(pwd)
BIN=$(dirname $0)
LIB=$BIN/../lib
[ -d $LIB ] || exit 1
[ -d /etc/${ME} ] && ETC=/etc/${ME} || ETC=$PWD/etc
[ -f $ETC/${ME}.conf ] && . $ETC/${ME}.conf
[ -z "$VAR" ] && VAR=$PWD/var
mkdir -p $VAR/documents/drafts $VAR/documents/pdf
