$OutFile=shift(@ARGV);
$InStr=shift(@ARGV);

@Tmp=split('_',$InStr);
$Chapter=shift(@Tmp);
$Level=shift(@Tmp);
$LevelDir =$Chapter/\$Level;

# printf("I got\n0: $OutFile\n1: $InStrn\n");
# printf("Chapter  = $Chapter\n");
# printf("Level    = $Level\n");
# printf("LevelDir = $LevelDir\n");

$OutFile=">$OutFile";
open(OutFile) || die "Can't create makefile $OutFile; $!";
print OutFile <<eot
#print  <<eot
.PHONY : make$Chapter\_$Level clean$Chapter\_$Level

make$Chapter\_$Level\:\t$Chapter\_$Level\_LVL\n
clean$Chapter\_$Level\:\tclean$Chapter\_$Level\_LVL

$Chapter\_$Level\_IN_DIR :=\t\$(LEVELS_IN_DIR)/$Chapter/$Level
$Chapter\_$Level\_IN  :=\t\$(foreach LVL, \$(LEVELS_$Chapter\_$Level), \$($Chapter\_$Level\_IN_DIR)/$Level\$(LVL).mex)
$Chapter\_$Level\_OUT :=\t\$(foreach LVL, \$(LEVELS_$Chapter\_$Level), \$(LEVELS_OUT_DIR)/$Chapter\_$Level\$(LVL).lvl)
$Chapter\_$Level\_TEX :=\t\$(LEVELS_OUT_DIR)/$Chapter\_$Level.tex)

clean$Chapter\_$Level\_LVL :\n\t\$(RM) -f \$($Chapter\_$Level\_OUT) \$($Chapter\_$Level\_TEX)
$Chapter\_$Level\_LVL :\t\$($Chapter\_$Level\_IN)

\$($Chapter\_$Level\_OUT) : \$($Chapter\_$Level\_IN)
\t\@\$(MKLEVEL) \$($Chapter\_$Level\_IN) -o:\$(LEVELS_OUT_DIR) -c:$Chapter -l:$Level \$(LEVELS_OPTS)

eot
;
close(OutFile);
