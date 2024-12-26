$current_time = Get-Date -Format "yyyy-MM-dd_HH-mm-ss"
$output_file = "./results/test_highlevel_stitch/$current_time.jpg"


./bin/test_highlevel_stitch.exe `
--d3 `
--mode panorama `
--output $output_file `
./datasets/images/data3/01.png `
./datasets/images/data3/02.png