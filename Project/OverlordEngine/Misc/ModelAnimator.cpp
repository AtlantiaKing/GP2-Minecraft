#include "stdafx.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter):
	m_pMeshFilter{pMeshFilter}
{
	SetAnimation(0);
}

void ModelAnimator::Update(const SceneContext& sceneContext)
{
	//We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		//1. 
		//Calculate the passedTicks (see the lab document)
		auto passedTicks = sceneContext.pGameTime->GetElapsed() * m_CurrentClip.ticksPerSecond * m_AnimationSpeed;
		//Make sure that the passedTicks stay between the m_CurrentClip.Duration bounds (fmod)
		if(passedTicks > m_CurrentClip.duration) passedTicks = fmodf(passedTicks, m_CurrentClip.duration);

		//2.
		//IF m_Reversed is true
		//	Subtract passedTicks from m_TickCount
		//	If m_TickCount is smaller than zero, add m_CurrentClip.Duration to m_TickCount
		//ELSE
		//	Add passedTicks to m_TickCount
		//	if m_TickCount is bigger than the clip duration, subtract the duration from m_TickCount
		if (m_Reversed)
		{
			m_TickCount -= passedTicks;
			if (m_TickCount < 0) m_TickCount += m_CurrentClip.duration;
		}
		else
		{
			m_TickCount += passedTicks;
			if (m_TickCount > m_CurrentClip.duration) m_TickCount -= m_CurrentClip.duration;
		}

		//3.
		//Find the enclosing keys
		AnimationKey keyA{}, keyB{};
		//Iterate all the keys of the clip and find the following keys:
		//keyA > Closest Key with Tick before/smaller than m_TickCount
		//keyB > Closest Key with Tick after/bigger than m_TickCount
		for (int i{}; i < m_CurrentClip.keys.size(); ++i)
		{
			const auto& key{ m_CurrentClip.keys[i] };

			if (key.tick > m_TickCount)
			{
				keyB = key;
				keyA = m_CurrentClip.keys[i - 1];
				break;
			}
		}

		//4.
		//Interpolate between keys
		//Figure out the BlendFactor (See lab document)
		const float blendFactor{ 1 - (keyB.tick - m_TickCount) / (m_CurrentClip.duration / (m_CurrentClip.keys.size() - 1)) };
		
		//Clear the m_Transforms vector
		//FOR every boneTransform in a key (So for every bone)
		//	Retrieve the transform from keyA (transformA)
		//	auto transformA = ...
		// 	Retrieve the transform from keyB (transformB)
		//	auto transformB = ...
		//	Decompose both transforms
		//	Lerp between all the transformations (Position, Scale, Rotation)
		//	Compose a transformation matrix with the lerp-results
		//	Add the resulting matrix to the m_Transforms vector
		for (int i{}; i < m_pMeshFilter->GetBoneCount(); ++i)
		{
			const auto& transformA{ keyA.boneTransforms[i] };
			const auto& transformB{ keyB.boneTransforms[i] };

			XMVECTOR scaleA;
			XMVECTOR rotationA;
			XMVECTOR translationA;
			XMMatrixDecompose(&scaleA, &rotationA, &translationA, XMLoadFloat4x4(&transformA));

			XMVECTOR scaleB;
			XMVECTOR rotationB;
			XMVECTOR translationB;
			XMMatrixDecompose(&scaleB, &rotationB, &translationB, XMLoadFloat4x4(&transformB));

			const XMVECTOR scaleLerp{ XMVectorLerp(scaleA, scaleB, blendFactor) };
			const XMVECTOR rotationSlerp{ XMQuaternionSlerp(rotationA, rotationB, blendFactor) };
			const XMVECTOR translationLerp{ XMVectorLerp(translationA, translationB, blendFactor) };

			const XMMATRIX transform{ XMMatrixScalingFromVector(scaleLerp) * XMMatrixRotationQuaternion(rotationSlerp) * XMMatrixTranslationFromVector(translationLerp) };
			//const XMMATRIX transform{ XMMatrixTransformation(translationLerp, rotationSlerp, scaleLerp, translationLerp, rotationSlerp, translationLerp) };

			XMStoreFloat4x4(&m_Transforms[i], transform);
		}
	}
}

void ModelAnimator::SetAnimation(const std::wstring& clipName)
{
	//Set m_ClipSet to false
	m_ClipSet = false;
	 
	//Iterate the m_AnimationClips vector and search for an AnimationClip with the given name (clipName)
	//If found,
	//	Call SetAnimation(Animation Clip) with the found clip
	//Else
	//	Call Reset
	//	Log a warning with an appropriate message
	for (auto& animationClip : m_pMeshFilter->GetAnimationClips())
	{
		if (animationClip.name == clipName)
		{
			SetAnimation(animationClip);
			return;
		}
	}

	Reset();
	Logger::LogWarning(L"No animation has been found with name " + clipName);
}

void ModelAnimator::SetAnimation(UINT clipNumber)
{
	//Set m_ClipSet to false
	m_ClipSet = false;

	//Check if clipNumber is smaller than the actual m_AnimationClips vector size
	//If not,
		//	Call Reset
		//	Log a warning with an appropriate message
		//	return
	//else
		//	Retrieve the AnimationClip from the m_AnimationClips vector based on the given clipNumber
		//	Call SetAnimation(AnimationClip clip)
	const auto animations{ m_pMeshFilter->GetAnimationClips() };
	if (clipNumber < animations.size() && clipNumber >= 0)
	{
		SetAnimation(animations[clipNumber]);
		return;
	}

	Reset();
	Logger::LogWarning(L"No animation has been found with index " + clipNumber);
}

void ModelAnimator::SetAnimation(const AnimationClip& clip)
{
	//Set m_ClipSet to true
	m_ClipSet = true;
	
	//Set m_CurrentClip
	m_CurrentClip = clip;

	//Call Reset(false)
	Reset(false);
}

void ModelAnimator::Reset(bool pause)
{
	//If pause is true, set m_IsPlaying to false
	if (pause) m_IsPlaying = false;

	//Set m_TickCount to zero
	//Set m_AnimationSpeed to 1.0f
	m_TickCount = 0.0f;
	m_AnimationSpeed = 1.0f;

	//If m_ClipSet is true
	//	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)
	//	Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assign)
	//Else
	//	Create an IdentityMatrix 
	//	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at vector::assign)
	if (m_ClipSet)
	{
		m_Transforms = m_CurrentClip.keys[0].boneTransforms;
		return;
	}
	
	const XMFLOAT4X4 identity
	{ 
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	m_Transforms = std::vector<XMFLOAT4X4>(m_pMeshFilter->GetBoneCount(), identity);
}
