#!/bin/bash
#
# cylinder_bar [percent [image]]
#
# Create a Cylindrical Percentage Bar and Label
#
# Method makes use of a shading overlay to generate more realisitic
# glass effects, especially at the ends of the 'glass' cylinder.
# It uses a drawn layers approach.
#   * White background layer (also providing the shape for later shadows)
#   * Draw over with green 3D cylinder of the right lenght
#   * Overlaid with a Transparent shaded glass effect.
#   * Final edge drawing of the glass cylinder
#   * Shadow effects of the whole result
#   * A shadowed label, generated and appended as a separate image
#
####
#
# Anthony Thyssen   30 November 2010
#
# Using a technique for generating cylinders using rounded rectangles
# originally demonstrated by "Totor" on ImageMagick Users Forum...
#   http://www.imagemagick.org/discourse-server/viewtopic.php?f=1&p=66100
#
# Warning this makes use of some fancy BASH shell script methods running
# sub-processes to generating the draw commands read into the primary command
# via bash inline named pipes. That is the construct "<(...)" is replaced by a
# named pipe, which is basically like a temporary file containing either drawing
# commands which were echo'ed, or a generated image.  Very tricky
#
percent="${1:-"20"}"

width=320
height=100

offset=10    # offset from edges, large enough to hold shadow
thickness=2  # glass thickness betwwn outside and colored inside cylinder

# The bar colors (before glass shading)
bar_color1=lime
bar_color2=green
bar_color3=green2  # end cap color (before glass shading)

background=White   # The final background color to use (can be "none")
                   # Alternatives:  Lavendar, Gainsboro, LemonChiffon

# Now calculate the other constants of the cyliner from the above.
glass_radius=$((height/2-offset))              # 40
glass_endcap=$((glass_radius/2))               # 20
glass_length=$((width-glass_radius-2*offset))  # 260 - excludes endcaps

bar_radius=$((glass_radius-2*thickness))
bar_endcap=$((bar_radius/2))
bar_length=$((glass_length*percent/100))


draw_base() {
  # This command is basically one enormouse 'echo' but is designed
  # so that when used with  <(...)   BASH will read it as if it
  # was comming from a external file of  MVG image file
  cat<<EOF
  # Draw the background base of the glass cylinder
  #
  # Translate all coordinates to be relative to center of end
  translate $((offset+glass_endcap)) $((height/2))
  # Create white base image for all cylinders
  fill white stroke snow3 stroke-width 1
  roundrectangle -$glass_endcap,-$glass_radius
                      $((glass_length+glass_endcap)),$glass_radius
                      $glass_endcap,$glass_radius
  # Draw left endcap ellipse of glass tube
  fill none  stroke snow3  stroke-width 1
  roundrectangle -$glass_endcap,-$glass_radius
                      $glass_endcap,$glass_radius
                      $glass_endcap,$glass_radius

  # Draw percentage bar on top of that base
  #
  # Gradient for percentage bar (a light gradient)
  # You really should have more control of the gradient generation than this!
  push defs
  push gradient 'bar_gradient' linear 0,0 0,$height
  stop-color $bar_color1   0%
  stop-color $bar_color2 100%
  pop gradient
  pop defs
  # Draw bar - main body
  push graphic-context
  fill 'url(#bar_gradient)' stroke snow4 stroke-width 1
  roundrectangle -$bar_endcap,-$bar_radius
                      $((bar_length+bar_endcap)),$bar_radius
                      $bar_endcap,$bar_radius
  pop graphic-context
  # Draw Bar - endcap
  translate $bar_length 0
  fill $bar_color3 stroke snow4 stroke-width 1
  roundrectangle -$bar_endcap,-$bar_radius
                      $bar_endcap,$bar_radius
                      $bar_endcap,$bar_radius
EOF
}

draw_glass_shading() {
  # This command is basically one enormouse 'echo' but is designed
  # so that when used with  <(...)  BASH will read it as if it
  # was comming from a external file of  MVG image file
  cat <<EOF
  # Draw a shading image for the glass transparency
  #
  # NOTE: The image generated is a shading mask.
  # As such black will be transparent, white will become opaque-black
  #
  # Translate all coordinates to be relative to center of end
  translate $((offset+glass_endcap)) $((height/2))
  # Gradient for glass shading of inside contents
  push defs
  push gradient 'glass_gradient' linear 0,0 0,$height
  stop-color 'black'   0%
  stop-color 'gray60' 100%
  pop gradient
  pop defs
  # Draw the shading overlay for glass cylinder
  push graphic-context
  fill 'url(#glass_gradient)'
  roundrectangle -$glass_endcap,-$glass_radius
      $((glass_length+glass_endcap)),$glass_radius
      $glass_endcap,$glass_radius
  pop graphic-context
  # Now glass end a solid shade, almost fully transparent
  translate $glass_length 0
  fill gray20
  roundrectangle -$glass_endcap,-$glass_radius \
                  $glass_endcap,$glass_radius  \
                  $glass_endcap,$glass_radius
EOF
}

draw_glass_edges() {
  # This command is basically one enormouse 'echo' but is designed
  # so that when used with  <(...)  BASH will read it as if it
  # was comming from a external file of MVG draw commands.
  cat <<EOF
  # Draw the final overlay of foreground edges to the glass cylinder
  #
  # Translate all coordinates to be relative to center of end
  translate $((offset+glass_endcap)) $((height/2))
  # draw the shading overlay for glass cylinder
  fill None stroke snow4 stroke-width 2
  roundrectangle -$glass_endcap,-$glass_radius
                  $((glass_length+glass_endcap)),$glass_radius
                  $glass_endcap,$glass_radius
  # now make right end almost fully transparent
  translate $glass_length 0
  fill None stroke snow4 stroke-width 1
  roundrectangle -$glass_endcap,-$glass_radius
                  $glass_endcap,$glass_radius
                  $glass_endcap,$glass_radius
EOF
}
label_image() {
  # This command is basically one enormouse 'echo' but is designed
  # to generate  convert options that will create a 'label' image.
  # using   $(...)
  #
  #convert
  cat<<EOF
  -background None \( -size ${width}x${height} \
  -fill red -stroke snow4 -strokewidth 1   label:'$percent %' \
  -trim +repage \
  \( +clone -background firebrick3 -shadow 80x3+3+3 \
  \) +swap -background none -layers merge \)
EOF
}

# ------------------
# Main program...

# Put it all together...
eval "convert -size ${width}x${height} -background None mvg:<(draw_base) \
        \( mvg:<(draw_glass_shading) -background black -alpha shape \
        \) -composite \
        -draw @<(draw_glass_edges) \
        \
        -trim +repage \
        \( +clone -background snow4 -shadow 80x3+3+3 \
        \) +swap -background none -layers merge +repage \
        \
        $(label_image) \
        -gravity center -insert 0 -append \
        -background $background -extent ${width}x$((2*height)) \
        '${2:-"show:"}'"

exit 0

# Replace the last group of lines with this to Remove label completly
        \
        -background $background -extent ${width}x${height} \
        "${2:-"show:"}"

# or these to place the label on top of the cylinder
        \
        $(label_image) \
        -background none -gravity center -extent ${width}x${height} \
        -composite -bordercolor $background -border 0 \
        "${2:-"show:"}"

