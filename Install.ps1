echo "first we need the java directory"
Add-Type -AssemblyName System.Windows.Forms
$JavaJdk = New-Object System.Windows.Forms.FolderBrowserDialog -Property @{
        SelectedPath =  '\Program Files (x86)'
        ShowNewFolderButton = $false
        Description = "Please locate the '\java\jdk8_*' Directory "
        
    }
[void]$JavaJdk.ShowDialog()
$mingW = New-Object System.Windows.Forms.FolderBrowserDialog -Property @{
        SelectedPath = '\Program Files (x86)'
        ShowNewFolderButton =$false
        Description = "Please Select the '\mingw' Directory"
        }
[void]$mingW.ShowDialog()
$jdkbin = ";"+$JavaJdk.SelectedPath+"\bin"
$jreBinS = ";"+$JavaJdk.SelectedPath+"\jre\bin\server"
$jreBinP = ";"+$JavaJdk.SelectedPath+"\jre\bin\plugin2"
$mingwBin = ";"+$mingW.SelectedPath+"\bin"
$addedPaths = $jdkbin+$jreBinS+$jreBinP+$mingwBin

[Environment]::SetEnvironmentVariable("Path",$env.Path+$addedPaths,[System.EnvironmentVariableTarget]::Machine)