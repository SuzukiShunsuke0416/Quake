#include "Particle.hlsli"
Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float4 Blur(float2 uv, int count)
{
	float power = diffuse.y;
	float size = diffuse.z * count;
	float rat = diffuse.w;//‰æ–Ê”ä’²®

	float4 output = 0;
	for (uint i = 0; i < 9; i+=2)
	{
		output += tex.Sample(samLinear, uv
			+ float2((float(i % 3) - 1.0f) * size * rat, (float(i / 3) - 1.0f) * size)) * power;
	}
	output += tex.Sample(samLinear, uv);

	return output;

}


float4 main(PS_INPUT input) : SV_TARGET
{
	float4 BaseColor = tex.Sample(samLinear, input.Tex);
	float4 output = 0;

	for (uint i = 1; i <= 10; i++) {
		output += Blur(input.Tex,i);
	}

	// Œ³‚Ì•`‰æˆ—‚ğ‚­‚Á‚«‚è•\¦‚³‚¹‚é‚©
	output = lerp(BaseColor, output, step(BaseColor.a, diffuse.x));


	return output;
}