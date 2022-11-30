rm -rf unzips/*

for file in zips/*.zip
    set name (basename $file .zip)
    echo $name
    mkdir -f unzips/$name
    # cp $file unzips/$name
    unzip $file -d unzips/$name

    set nesfile ( find unzips/$name -name "*.nes")
    echo  $nesfile[1] "->" nes/$name.nes
    cp -f $nesfile[1] nes/$name.nes
end
