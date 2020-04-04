#! /bin/sed
# convert zh to en
# sed -f ~/sed_zh2en.sed -i file.c
1,$ s#，#,#g
1,$ s#。#;#g
1,$ s#；#;#g
1,$ s#：#:#g
1,$ s#？#?#g
1,$ s#”#"#g
1,$ s#“#"#g
1,$ s#（#(#g
1,$ s#）#)#g
1,$ s#～#~#g
1,$ s#！#!#g
1,$ s#、#|#g
1,$ s#／#/#g
1,$ s#．#. #g
1,$ s#【#[#g
1,$ s#】#]#g
1,$ s#－#-#g

