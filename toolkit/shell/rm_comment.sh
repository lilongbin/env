#! /bin/sed -f
## Longbin <beangr@163.com>
## 2015-07-23

function awk_replace_c_comment()
{
	cat $* | awk 'BEGIN{RS=""} {gsub(/\/\*[^*]*\*+([^/*][^*]*\*+)*\//, " ", $0); gsub(/\/\/[^\n]*/, "", $0); print $0}'
}

function awk_replace_comment()
{
	FILES=$*
	for file in ${FILES}
	do
		echo "////////// ${file} start ///////////////////////////////////"
		awk_replace_c_comment ${file}
		echo "////////// ${file} end /////////////////////////////////////"
	done
}
awk_replace_comment $*

