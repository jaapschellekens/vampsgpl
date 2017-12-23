% Site specific Slang settings

#ifdef AGL
% Set the AGL environment variable. Note that the trailing backslash
% is a must.
if (strcmp(getenv("VAMPSLIB"),"") != 0){
	putenv(sprintf("AGL3CONFIG=%s/",getenv("VAMPSLIB"),1));
}else{
	putenv("AGL3CONFIG=/usr/local/share/vamps0.99i/");
}
#endif
