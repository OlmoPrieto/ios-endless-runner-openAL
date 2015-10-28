//
//  Shader.fsh
//  EndlessRunner
//
//  Created by Marcos Marti Nacher on 2/10/15.
//  Copyright © 2015 ESAT. All rights reserved.
//

precision mediump float;

uniform vec3 u_color;
uniform sampler2D u_texture;
uniform int has_texture;

varying vec2 o_uv;

void main()
{
    vec3 asd = u_color;
    if (has_texture == 1) {
        gl_FragColor = texture2D(u_texture, o_uv);
    } else {
        gl_FragColor = vec4(u_color, 1.0);
    }
    //gl_FragColor = vec4(1.0, 0.8, 0.0, 1.0);
}
