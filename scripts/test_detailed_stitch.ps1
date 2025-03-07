$current_time = Get-Date -Format "yyyy-MM-dd_HH-mm-ss"
$output_file = "./results/test_detailed_stitch/$current_time.jpg"

./bin/test_detailed_stitch.exe `
F:/MasterGraduate/03-Code/PanoramicTracking/datasets/images/data5/3.jpg `
F:/MasterGraduate/03-Code/PanoramicTracking/datasets/images/data5/4.jpg `
--output $output_file `
--try_cuda yes `
--work_megapix 0.6 `
--features akaze `
--matcher homography `
--estimator homography `
--match_conf 0.5 `
--conf_thresh 1.0 `
--ba ray `
--ba_refine_mask x_xxx `
--wave_correct horiz `
--warp spherical `
--seam_megapix 0.1 `
--seam gc_color `
--compose_megapix -1 `
--expos_comp gain_blocks `
--expos_comp_nr_feeds 1 `
--expos_comp_nr_filtering 2 `
--expos_comp_block_size 32 `
--blend multiband `
--blend_strength 5 `
--rangewidth 10

# --timelapse as_is `
# --preview `

# F:/MasterGraduate/03-Code/PanoramicTracking/datasets/images/data5/1.jpg `
# F:/MasterGraduate/03-Code/PanoramicTracking/datasets/images/data5/2.jpg `
# F:/MasterGraduate/03-Code/PanoramicTracking/datasets/images/data5/3.jpg `
# F:/MasterGraduate/03-Code/PanoramicTracking/datasets/images/data5/4.jpg `
# F:/MasterGraduate/03-Code/PanoramicTracking/datasets/images/data5/5.jpg `

# ./datasets/images/data4/9.jpg `

<#
Flags:
  --preview
      Run stitching in the preview mode. Works faster than usual mode,
      but output image will have lower resolution.
  --try_cuda (yes|no)
      Try to use CUDA. The default value is 'no'. All default values
      are for CPU mode.

Motion Estimation Flags:
  --work_megapix <float>
      Resolution for image registration step. The default is 0.6 Mpx.
  --features (surf|orb|sift|akaze)
      Type of features used for images matching.
      The default is surf if available, orb otherwise.
  --matcher (homography|affine)
      Matcher used for pairwise image matching.
  --estimator (homography|affine)
      Type of estimator used for transformation estimation.
  --match_conf <float>
      Confidence for feature matching step. The default is 0.65 for surf and 0.3 for orb.
  --conf_thresh <float>
      Threshold for two images are from the same panorama confidence.
      The default is 1.0.
  --ba (no|reproj|ray|affine)
      Bundle adjustment cost function. The default is ray.
  --ba_refine_mask (mask)
      Set refinement mask for bundle adjustment. It looks like 'x_xxx',
      where 'x' means refine respective parameter and '_' means don't
      refine one, and has the following format:
      <fx><skew><ppx><aspect><ppy>. The default mask is 'xxxxx'. If bundle
      adjustment doesn't support estimation of selected parameter then
      the respective flag is ignored.
  --wave_correct (no|horiz|vert)
      Perform wave effect correction. The default is 'horiz'.
  --save_graph <file_name>
      Save matches graph represented in DOT language to <file_name> file.
      Labels description: Nm is number of matches, Ni is number of inliers,
      C is confidence.

Compositing Flags:
  --warp (affine|plane|cylindrical|spherical|fisheye|stereographic|compressedPlaneA2B1|compressedPlaneA1.5B1|compressedPlanePortraitA2B1|compressedPlanePortraitA1.5B1|paniniA2B1|paniniA1.5B1|paniniPortraitA2B1|paniniPortraitA1.5B1|mercator|transverseMercator)
      Warp surface type. The default is 'spherical'.
  --seam_megapix <float>
      Resolution for seam estimation step. The default is 0.1 Mpx.
  --seam (no|voronoi|gc_color|gc_colorgrad)
      Seam estimation method. The default is 'gc_color'.
  --compose_megapix <float>
      Resolution for compositing step. Use -1 for original resolution.
      The default is -1.
  --expos_comp (no|gain|gain_blocks|channels|channels_blocks)
      Exposure compensation method. The default is 'gain_blocks'.
  --expos_comp_nr_feeds <int>
      Number of exposure compensation feed. The default is 1.
  --expos_comp_nr_filtering <int>
      Number of filtering iterations of the exposure compensation gains.
      Only used when using a block exposure compensation method.
      The default is 2.
  --expos_comp_block_size <int>
      BLock size in pixels used by the exposure compensator.
      Only used when using a block exposure compensation method.
      The default is 32.
  --blend (no|feather|multiband)
      Blending method. The default is 'multiband'.
  --blend_strength <float>
      Blending strength from [0,100] range. The default is 5.
  --output <result_img>
      The default is 'result.jpg'.
  --timelapse (as_is|crop)
      Output warped images separately as frames of a time lapse movie, with 'fixed_' prepended to input file names.
  --rangewidth <int>
      uses range_width to limit number of images to match with.
#>