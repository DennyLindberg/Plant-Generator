#pragma once
#include "lsystem.h"
#include "canvas.h"
#include "turtle2d.h"
#include "turtle3d.h"
#include "core/randomization.h"

void DrawFractalTree(Canvas2D& canvas, int iterations, float scale = 10.0f, glm::fvec2 origin = glm::fvec2{ 0.0f }, float startAngle = 90);
void DrawKochCurve(Canvas2D& canvas, int iterations, float scale = 10.0f, glm::fvec2 origin = glm::fvec2{ 0.0f }, float startAngle = 90);
void DrawSierpinskiTriangle(Canvas2D& canvas, int iterations, float scale = 10.0f, glm::fvec2 origin = glm::fvec2{ 0.0f }, float startAngle = 90);
void DrawDragonCurve(Canvas2D& canvas, int iterations, float scale = 10.0f, glm::fvec2 origin = glm::fvec2{ 0.0f }, float startAngle = 90);
void DrawFractalPlant(Canvas2D& canvas, int iterations, float scale = 10.0f, glm::fvec2 origin = glm::fvec2{ 0.0f }, float startAngle = 90);
void DrawFractalTreeNezumiV1(Canvas2D& canvas, int iterations, float scale = 10.0f, glm::fvec2 origin = glm::fvec2{ 0.0f }, float startAngle = 90);
void DrawFractalTreeNezumiV2(Canvas2D& canvas, int iterations, float scale = 10.0f, glm::fvec2 origin = glm::fvec2{ 0.0f }, float startAngle = 90);
void DrawFractalTreeNezumiV3(Canvas2D& canvas, int iterations, float scale = 10.0f, glm::fvec2 origin = glm::fvec2{ 0.0f }, float startAngle = 90);
void DrawFractalLeaf(std::vector<glm::fvec3>& generatedHull, Canvas2D& canvas, Color color, int iterations, float scale = 10.0f, glm::fvec2 origin = glm::fvec2{ 0.0f }, float startAngle = 90);

template<class T>
void GenerateFractalPlant3D(Turtle3D<T>& turtle, UniformRandomGenerator& uniformGenerator, int iterations, float scale = 0.1f)
{
	using Turtle = Turtle3D<T>;
	LSystemStringFunctional fractalTree;
	fractalTree.axiom = "0";
	fractalTree.productionRules['0'] = [&uniformGenerator]() -> std::string
	{
		std::string result = "1[0]";

		if (uniformGenerator.RandomFloat() < 0.5f)
		{
			result += "[0]";
		}

		return result + "0";
	};
	fractalTree.productionRules['1'] = [&uniformGenerator]() -> std::string
	{
		return "11";
	};

	turtle.actions['0'] = [scale, &uniformGenerator](Turtle& t, int repetitions)
	{
		float forwardGrowth = 0.0f;
		while (--repetitions >= 0)
		{
			forwardGrowth += uniformGenerator.RandomFloat();
		}
		forwardGrowth *= scale;

		t.MoveForward(forwardGrowth);
	};
	turtle.actions['1'] = turtle.actions['0'];
	turtle.actions['['] = [scale, &uniformGenerator](Turtle& t, int repetitions)
	{
		t.PushState();
		t.Rotate(180.0f*uniformGenerator.RandomFloat(0.1f, 1.0f),
			45.0f*uniformGenerator.RandomFloat(0.2f, 1.0f));
	};
	turtle.actions[']'] = [scale, &uniformGenerator](Turtle& t, int repetitions)
	{
		t.PopState();
		t.Rotate(-180.0f*uniformGenerator.RandomFloat(0.1f, 1.0f),
			45.0f*uniformGenerator.RandomFloat(0.2f, 1.0f));
	};

	turtle.GenerateSkeleton(fractalTree.RunProduction(iterations));
}


struct FractalTree3DProps
{
	int depth = 0;				// How far the node is from the root
	float lengthFactor = 1.0f;
	float thickness = 1.0f;
};

struct FractalBranch
{
	using TBone = Bone<FractalTree3DProps>;

	std::vector<TBone*> nodes;

	FractalBranch(TBone* root)
	{
		Push(root);
	}

	void Push(TBone* node)
	{
		nodes.push_back(node);
	}

	bool IsLeaf()
	{
		return (nodes.size() == 1);
	}
};

void GenerateFractalTree3D(UniformRandomGenerator& uniformGenerator, int iterations, std::function<void(Bone<FractalTree3DProps>*, std::vector<FractalBranch>&)> onResultCallback);
