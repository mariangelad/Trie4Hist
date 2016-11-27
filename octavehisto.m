#! /usr/bin/octave -qf 

files=glob("*hist");
nfiles=size(files,1);

base= floor(sqrt(nfiles));
altezza = ceil(nfiles/base);

for i=1:nfiles
    vect=load(files{i});
    subplot(base, altezza, i);
    bar(vect(:,1),vect(:,2), 1.0, "hist", "facecolor","w", "edgecolor", "b");
    title(files{i});
endfor;


print("histograms.png", "-dpng");

