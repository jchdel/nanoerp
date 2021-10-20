_balance(){
	BALANCE=$(cat $1 2>/dev/null | while read line
	do
		SIGN=$(echo $line | cut -d\; -f2)
		AMOUNT=$(echo $line | cut -d\; -f4)
		_is_number $AMOUNT
		[ x"$SIGN" = "xC" ] && TOTAL=$(echo "$TOTAL+$AMOUNT"|bc)
		[ x"$SIGN" = "xD" ] && TOTAL=$(echo "$TOTAL-$AMOUNT"|bc)
		echo "$TOTAL"
	done | tail -1)
	[ -z "$BALANCE" ] && BALANCE=0
	echo "$BALANCE"
}

########################################################################

[ -d $VAR ] || exit 1

JOURNALS=$VAR/comptabilite/journaux
mkdir -p $JOURNALS

BOOK=$VAR/comptabilite/grand-livre
for i in {1..7}
do
	mkdir -p $BOOK/$i
done

