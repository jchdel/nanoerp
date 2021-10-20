_add(){
	[ -z "$NAME" ] && echo "$NAME is missing." && _exit 1
	[ -f ${VAR}/${ME}s/by-name/"$NAME" ] && echo "$NAME already exists." >&2 && _exit 1
	ID=$(date|md5sum|cut -d\  -f1)
	mkdir -p ${VAR}/${ME}s/by-name
	echo "class=\"${ME}\"" >> ${VAR}/${ME}s/$ID
	echo "id=\"$ID\"" >> ${VAR}/${ME}s/$ID
	echo "name=\"$NAME\"" >> ${VAR}/${ME}s/$ID
	echo "creation_date=\"$(date)\"" >> ${VAR}/${ME}s/$ID
	echo "creator=\"$(whoami)@$(hostname)\"" >> ${VAR}/${ME}s/$ID
	echo "history=()" >> ${VAR}/${ME}s/$ID
	echo "proposal=()" >> ${VAR}/${ME}s/$ID
	[ -z "$PARAMS" ] || echo "params=\"$PARAMS\"" >> ${VAR}/${ME}s/$ID
	ln -s ../$ID ${VAR}/${ME}s/by-name/"$NAME"
	echo "$ID"
}

_view(){
	[ -z "$NAME" ] && echo "NAME or ID is missing." >&2 && _exit 1
	cat ${VAR}/${ME}s/by-name/"$NAME" 2>/dev/null && _exit 0 || \
	cat ${VAR}/${ME}s/"$NAME" 2>/dev/null && _exit 0 || \
	echo "$NAME not found" >&2 && _exit 1
}

_check(){
	[ -z "$NAME" ] && echo "NAME or ID is missing." >&2 && _exit 1
	[ -f "${VAR}/${ME}s/by-name/$NAME" ] && . ${VAR}/${ME}s/by-name/"$NAME"
	[ -f "${VAR}/${ME}s/$NAME" ] && . ${VAR}/${ME}s/"$NAME"
	[ -z "$name" ] && echo "NAME not found for $NAME." >&2 && _exit 1
	[ -z "$id" ] && echo "ID not found for $NAME" >&2 && _exit 1
	return 0
}

_del(){
	_check
	rm -f ${VAR}/${ME}s/by-name/"$name" ${VAR}/${ME}s/"$id"
	echo "${ME} $name ($id) is deleted."
}

_edit(){
	[ -z "$NAME" ] && echo "NAME or ID is missing." >&2 && _exit 1
	if [ -f "${VAR}/${ME}s/by-name/$NAME" ] ; then
		vi ${VAR}/${ME}s/by-name/"$NAME" 
	elif [ -f "${VAR}/${ME}s/$NAME" ]; then
		vi ${VAR}/${ME}s/"$NAME"
	else
		echo "$NAME not found" >&2
		_exit 1
	fi
}

_history(){
	_check
	for i in "${history[@]}";do echo $i;done
}

_list(){
	if [ -z "$NAME" ]; then
		ls -1 ${VAR}/${ME}s/ | grep -v by-name | sort
	else
		ls -1 ${VAR}/${ME}s/by-name/ | sort
	fi
}

_get(){
	[ -z "$PARAMS" ] && echo "Please select an attribute." >&2 && _exit 1
	_check
	ATTR=$(echo "${PARAMS}"|cut -d\  -f1)
	grep "^${ATTR}=" ${VAR}/${ME}s/$id | cut -d= -f2| sed 's/"//g'
}

_set(){
	[ -z "$PARAMS" ] && echo "Please select an attribute." >&2 && _exit 1
	_check
	ATTR=$(echo "${PARAMS}"|cut -d\  -f1)
	VALUE=$(echo "${PARAMS}"|cut -d\  -f2-)
	sed -i -e "/^${ATTR}/d" ${VAR}/${ME}s/$id
	echo "${ATTR}=\"${VALUE}\"" >> ${VAR}/${ME}s/$id
	_get
}

_client(){
	_check
	$BIN/client view $client || _view
}

_log(){
	_check
	[ -z "$history" ] && history=()
	NHIST=${#history[@]}
	sed -i -e '/^history=/d' ${VAR}/${ME}s/$id
	history[$NHIST]="$(date) $PARAMS"
	(
		echo -n "history=("
		for i in "${history[@]}"
		do
			echo -n "\"$i\" "
		done
		echo ")"
	) >> ${VAR}/${ME}s/$id
}

_usage() {
	echo "$ME <ACTION> [<NAME> [<PARAMS>]]"
	echo
	echo "ACTION:"
	echo "add NAME      : create a new ${ME} entrypoint by NAME"
	echo "view NAME     : view raw attributes by NAME or ID"
	echo "del NAME      : delete a ${ME} by NAME or ID"
	echo "edit NAME     : edit (vi) raw ${ME} file by NAME or ID"
	echo "history NAME  : list history array by NAME or ID"
	echo "list          : list ${ME}s by ID"
	echo "list *        : list ${ME}s by NAME"
	echo "get NAME ATTR : get ATTRibute by NAME or ID"
	echo "set NAME ATTR VALUE : set ATTRibute=\"VALUE\" by NAME or ID"
	echo "client NAME   : view raw attributes of client of NAME or ID"
	echo "log NAME STRING : add STRING to history of NAME or ID"
}

########################################################################

[ $# -gt 0 ] && ACTION=$1 && shift || ACTION=error
[ $# -gt 0 ] && NAME=$1 && shift
[ $# -gt 0 ] && PARAMS=$*
FLAG0=1
case $ACTION in
	"add")
		_add
		;;
	"view")
		_view
		;;
	"del")
		_del
		;;
	"edit")
		_edit
		;;
	"history")
		_history
		;;
	"list")
		_list
		;;
	"get")
		_get
		;;
	"set")
		_set $PARAMS
		;;
	"client")
		_client
		;;
	"log")
		_log
		;;
	"help") 
		_usage
		;;
	*)
		FLAG0=0
		;;
esac
