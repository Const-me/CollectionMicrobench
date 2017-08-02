del result-win.tsv
for /L %%i in (0, 1, 10) do x64\Release\CollectionMicrobench.exe %%i >> result-win.tsv
