
/*
#defines are set in host code and
prepended to file after I/O read

Includes stuff like parameters and UI flags
--------------------------
Zachary Shore
Fractal Raymarching
*/

#define M_PI 3.1415926535897932384626433832795

#ifdef GL_ES
precision highp float;
#endif

uniform vec2 u_resolution;
uniform float u_time;

// Camera
uniform vec3 u_cam_eye;
uniform vec3 u_cam_view;
uniform vec3 u_cam_up;
uniform vec3 u_cam_right;
uniform float u_cam_fov;

in vec2 v_texcoord;

// Colormaps
#if 1
uniform vec3 colormap[4] = {
    vec3(0.5),
    vec3(0.5),
    vec3(1.0),
    vec3(0.0, 0.1, 0.2)
};
#endif

vec3 sky_color = vec3(0.025);

vec3 core_color  = vec3(0.882, 0.424, 0.140);
vec3 outer_color = vec3(0.157, 0.114, 0.067);
vec3 inner_color = vec3(0.454, 0.204, 0.196);

vec3 fogColor = vec3(0.718, 0.847, 0.914);
vec3 glow_color = vec3(0.718, 0.847, 0.914);

vec3 lgt_eye = u_cam_eye;
vec3 lgt_color = vec3(1.0);

vec4 scalevec = vec4(SCALE, SCALE, SCALE, abs(SCALE)) / MR2;
float C1 = abs(SCALE - 1);
float C2 = pow(abs(SCALE), float(1 - FRACTAL_ITERS));

float scalefactor = 1.0;//mix( 2.0*(SCALE + 1.0f)/(SCALE - 1.0f), mix( 2.0, 1.0f, step(-1.0f, SCALE)), 1.0f - step(1.0f, SCALE));

// float scalefactor = 1.0;
float inverse_scale = (1.0f / scalefactor);
float bv_radius = length( vec3(scalefactor) );

//float FudgeFactor = 0.70588;
//float Dither = 0.37719;

float FudgeFactor = 0.75;
//float FudgeFactor = 1.0;
float Dither = 0.5;
// float Dither = 0.0;

float rand(vec2 co){
    // implementation found at: lumina.sourceforge.net/Tutorials/Noise.html
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void sphereFold(inout vec4 z) {
    float r2 = dot(z.xyz, z.xyz);

   //  z *= mix(
   //      1.0f / MR2, // Linear scaling inside
   //      mix(
   //          1.0f / r2, // Sphere inversion outside
   //          1.0f,
   //          step(1.0f, r2)
   //      ),
   //      step(MR2, r2)
   //  );

    float t = 1.0;
    if( r2 < MR2 )
        t = FR2 / MR2;
    else if( r2 < FR2 )
        t = FR2 / r2;

    z *= t;
}

void boxFold(inout vec4 z) {
   z.x = clamp(z.x, -FLIMIT.x, FLIMIT.x) * 2.0 - z.x;
   z.y = clamp(z.y, -FLIMIT.y, FLIMIT.y) * 2.0 - z.y;
   z.z = clamp(z.z, -FLIMIT.z, FLIMIT.z) * 2.0 - z.z;
}

float Menger(vec3 z)
{
    float r;
    int n = 0;

    vec3 MnOffset = vec3(1.4898,1.95918,1.10202);
    float MnScale = 3.0;

    z = abs(z);
    if (z.x < z.y){ z.xy = z.yx;}
    if (z.x < z.z){ z.xz = z.zx;}
    if (z.y < z.z){ z.yz = z.zy;}
    if (z.z < 1. / 3.){ z.z -= 2. * (z.z - 1. / 3.); }

    while (n < 5 && dot(z,z) < 100.0) {

        z = MnScale * (z - MnOffset) + MnOffset;


        // Fold
        z = abs(z);
        if (z.x < z.y){ z.xy = z.yx; }
        if (z.x < z.z){ z.xz = z.zx; }
        if (z.y < z.z){ z.yz = z.zy; }
        if (z.z < 1. / 3. * MnOffset.z) { z.z -= 2. * (z.z - 1. / 3. * MnOffset.z); }

        // r = dot(z-MnOffset, z-MnOffset);
        //orbitTrap = min(orbitTrap, abs(vec4(z,r)));

        n++;
    }
        // return (length(z)-sqrt(3.) ) * pow(SCALE, float(-n));

    return float(z.x - MnOffset.x) * pow(MnScale, float(-n));
}

float juliabox(in vec3 z, inout vec4 orbitTrap)
{
    vec3 p = vec3(z);
    vec3 C = JULIA_C;//vec3(1.97016, -0.03052, -0.1194);
    //vec3 flimit = vec3(1.0084, 1, 1.0084);

    orbitTrap = vec4(1e20f);
    float DEfactor = 1.0;
    float DEoffset = DE_OFFSET;//0.00262;
    vec3 Offset = JULIA_OFFSET;//vec3(0.55552, 0.48148, -0.1852);

    vec3 p1 = abs(p);
    float r2 = max(p1.x, max(p1.y, p1.z));
    for(int i = 0; i < FRACTAL_ITERS && r2 < 60.0; i++)
    {
        p = clamp(p, -FLIMIT, FLIMIT) * 2.0 - p;

        r2 = dot(p.xyz, p.xyz);
        float k = max(SCALE/r2, 1.0);
        p = p * k + C;
        DEfactor *= k;

        p1 = abs(p);
        r2 = max(p1.x, max(p1.y, p1.z));

        orbitTrap.xyz = min(p1, orbitTrap.xyz);
        orbitTrap.w = min(r2, orbitTrap.w);
    }

    return abs(0.5 * Menger(p - Offset) / DEfactor - DEoffset);
}

float mandelbox(vec3 z, inout vec4 C)
{
    vec4 p = vec4(z, 1.0);
    // vec4 p0 = vec4(1.0);
    // vec4 p0 = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 p0 = vec4(z, 1.0);

    C = vec4(1e20f);

    vec3 z1 = abs(p.xyz);
    float r2 = max(z1.x, max(z1.y, z1.z));
    int i;
    for(i = 0; i < FRACTAL_ITERS && r2 < 100.0; i++)
    {
        boxFold(p);
        sphereFold(p);

        p.xyz = SCALE * p.xyz + p0.xyz;
        p.w = p.w * abs(SCALE) + p0.w;

        z1 = abs(p.xyz);
        r2 = max(z1.x, max(z1.y, z1.z));

        C.xyz = min(abs(p.xyz), C.xyz);
        C.w = min( r2, C.w );
    }

    // C.w = sqrt(C.w);
    // return (length(p.xyz) - bv_radius) / abs(p.w);
    // return abs((length(p.xyz) - bv_radius) / p.w);
    vec3 b = vec3(2.0);
    // return (length(max(abs(p.xyz) - b, 0.0)) - bv_radius) / abs(p.w);
    return abs( (length(max(abs(p.xyz) - b, 0.0)) - bv_radius) / p.w );

/*
    for(int i = 0; i < FRACTAL_ITERS; i++)
    {
        ap=p.xyz;
        p.xyz = clamp(p.xyz, -1.25, 1.25) * 2.0 - p.xyz;

        float r2 = dot(p.xyz, p.xyz);
        float k = max(SCALE / r2, 1.0);
        p *= k;

        // p.xyz += vec3(0.0);

        C.xyz = min(abs(p.xyz), C.xyz);
        C.w = min( r2, C.w );
    }
    return abs(0.5 * abs(p.y))/p.w;
*/

    // // distance estimate
    // float r2 = 0.0f;
    // for (int i=0; i < FRACTAL_ITERS; i++)
    // {
    //     p.xyz = clamp(p.xyz, -FLIMIT, FLIMIT) * 2.0 - p.xyz;  // box fold: min3, max3, mad3

    //     r2 = dot(p.xyz, p.xyz);  // dp3

    //     p.xyzw *= clamp(max(MR2/r2, MR2), 0.0, 1.0);  // sphere fold: div1, max1.sat, mul4

    //     r2 = length(p.xyz);

    //     p.xyz = p.xyz * scalevec.xyz + p0.xyz;  // mad4
    //     p.w = p.w * scalevec.w + 1.0;

    //     C.xyz = min(abs(p.xyz), C.xyz);
    //     C.w = min( r2, C.w);
    // }

    // return (length(p.xyz) - C1) / abs(p.w) - C2;
}

float mandelbulb( vec3 p, inout vec4 orbitTrap )
{
    vec3 w = p;
    float m = dot(w, w);

    orbitTrap = vec4(abs(w),m);//vec4(1e20f);
    float dz = 1.0;

    for(int i = 0; i < FRACTAL_ITERS; ++i)
    {
        float m2 = m*m;
        float m4 = m2*m2;
        dz = 8.0*sqrt(m4*m2*m)*dz + 1.0;

        float x = w.x; float x2 = x*x; float x4 = x2*x2;
        float y = w.y; float y2 = y*y; float y4 = y2*y2;
        float z = w.z; float z2 = z*z; float z4 = z2*z2;

        float k3 = x2 + z2;
        float k2 = inversesqrt( k3*k3*k3*k3*k3*k3*k3 );
        float k1 = x4 + y4 + z4 - 6.0*y2*z2 - 6.0*x2*y2 + 2.0*z2*x2;
        float k4 = x2 - y2 + z2;

        w.x = p.x +  64.0*x*y*z*(x2-z2)*k4*(x4-6.0*x2*z2+z4)*k1*k2;
        w.y = p.y + -16.0*y2*k3*k4*k4 + k1*k1;
        w.z = p.z +  -8.0*y*k4*(x4*x4 - 28.0*x4*x2*z2 + 70.0*x4*z4 - 28.0*x2*z2*z4 + z4*z4)*k1*k2;

        orbitTrap = min( orbitTrap, vec4(abs(w),m) );

        m = dot(w,w);
        if( m > 4.0 )
            break;
    }

    return 0.25*log(m)*sqrt(m)/dz;
}


/*
// Juliabox //
float sdf(vec3 p, inout vec4 C)
{
    return juliabox(p, C);
}

float sdf(vec3 p)
{
    vec4 C = vec4(0.0);
    return juliabox(p, C);
}
*/

// Mandelbulb //
// float sdf(vec3 p, inout vec4 C)
// {
//     return mandelbulb(p, C);
// }

// float sdf(vec3 p)
// {
//     vec4 C = vec4(0.0);
//     return mandelbulb(p, C);
// }

// Mandelbox //
float sdf(vec3 p, inout vec4 C)
{
    // vec3 q = mod(p, vec3(2.01)) - 1.005;
    return mandelbox(p * scalefactor, C) * inverse_scale;
}

float sdf(vec3 p)
{
    // vec3 q = mod(p, vec3(2.01)) - 1.005;
    vec4 C = vec4(0.0);
    return mandelbox(p * scalefactor, C) * inverse_scale;
}

vec3 getNormal(vec3 p)
{
    vec2 h = vec2(EPSILON, 0.0f);
    return normalize(vec3(
        sdf(p + h.xyy) - sdf(p - h.xyy),
        sdf(p + h.yxy) - sdf(p - h.yxy),
        sdf(p + h.yyx) - sdf(p - h.yyx)
        ));
}

vec3 colorize(float t)
{
    return colormap[0] + colormap[1] * cos(2.0 * M_PI * (colormap[2] * t + colormap[3]));
}

float AO(vec3 p, float dt)
{
    vec3 n = getNormal(p);

    float ao = 0.0;
    float maxao = 0.0;
    for(int i = 0; i < NORMAL_AO_SAMPLES; ++i)
    {
        ao += sdf(n * i * dt + p);
        maxao += i * dt + dt;
    }
    return clamp(ao / (NORMAL_AO_SAMPLES * dt), 0.0, 1.0);
}

vec3 raymarch(vec3 r0, vec3 rd)
{
    vec3 color = sky_color;
    vec4 C = vec4(0.0);
    vec3 p;
    float t = 0.0, d = 0.0;

    float r2 = bv_radius;

    float tca = dot(-r0, rd);
    float d2 = dot(-r0, -r0) - tca * tca;
    float hit = 1.0f - step(r2, d2);
    float thc = sqrt(hit * (r2 - d2));

    //t = clamp(tca - thc, 0.0, tca + thc);

    int i, j = 0;
    float notthere = 1.0;
    float dt = 0.0;
    for(i = 0; i < MAX_ITERATIONS /*&& t < (tca + thc)*/; ++i)
    {
        p = r0 + rd * t;
        d = sdf(p, C);

        d *= FudgeFactor;

        if(i == 0) d *= (Dither * rand(rd.xy)) + (1.0 - Dither);
        t += d;

        float epsModified = t * EPSILON;
        if(d < epsModified)
        {
            t -= (epsModified - d);
            break;
        }
    }

    float r = C.w;
    vec3 sum = vec3(0.0);

    // C.xyz = C.xyz * C.xyz;
    // sum += mix(inner_color, outer_color, C.x );//* C.x);
    // sum += mix(inner_color, outer_color, C.y );//* C.y);
    // sum += mix(inner_color, outer_color, C.z );//* C.z);
    // sum /= 3.0;
    // color = mix(sum, core_color, r);

    // color = colorize( C.w );

    // r = dot(C.xyz, C.xyz) / length(C.xyz);
    // r = max(C.x, max(C.y, C.z));
    // r = length(C.xyz) / C.w;

    // Inf-norm coloring
    C = abs(C);
    r = max(C.x, max(C.y, C.z));
    // r = max(C.w, r);

    color = colorize(r);

    // color = vec3(r);

    // color = vec3(0.0, 0.0, 1.0);

    // color = sum;

    // color = vec3( 1.0 - (max(sum.x, sum.y), sum.z) );

    vec3 N = getNormal(p - rd * EPSILON * 2.0);
    vec3 toLgt = normalize(lgt_eye - p);
    float Id = dot( N, toLgt );
    vec3 H = normalize(toLgt + -rd);
    float Is = pow( dot(H, N), 50.0f );

    color = color * Id;
    color += vec3(1.0) * Is;
    color = clamp(color, 0.0, 1.0);

    // { // Ambient Occlusion
    //     float ao = AO(p, 0.02);
    //     // ao = exp(-ao);
    //     // color = mix( glow_color, color, 1.0 - ao );
    //     // color = vec3(ao);
    //     // color = colorize(ao);
    //     color *= ao;
    // }

    //color = mix(color, sky_color, notthere);

    // t = mix( 1.0f / EPSILON, t, step(EPSILON, t));
    float b = FOG_LEVEL;
    float fogStrength = 1.0f - exp(-t * b);
    //color = mix(color, fogColor, fogStrength);

    {
        float g = float(i) / float(MAX_ITERATIONS);
        g = 1.0f - g;
        g = pow(g, GLOW_STRENGTH);
        color = mix( vec3(0.99f), color, g );
        // color = colorize(g);
    }

    return color;
}

void main()
{
	vec2 p_uv = v_texcoord;
    p_uv = 2.0 * p_uv - 1.0;

    float aspect_ratio = float(u_resolution.x) / float(u_resolution.y);
    float focal_length = 1.0 / tan( u_cam_fov / 2.0 * M_PI / 180.0 );

    // aa_uv = aa_uv + 0.001 * vec2(random(aa_uv - p_uv.xx), random(aa_uv - p_uv.yy));

    vec3 rd = normalize(
        u_cam_view * focal_length +
        u_cam_up * p_uv.y +
        u_cam_right * p_uv.x * aspect_ratio
        );

    vec3 r0 = u_cam_eye;

    // March sample
    vec3 color = raymarch(r0, rd);

    fragment_color = vec4( color, 1.0 );
}
