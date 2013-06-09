void AABBTreeCollider::InitQuery(const Matrix4x4* world0, const Matrix4x4* world1)
{
	// Reset stats & contact status
	Collider::InitQuery();
	mNbBVBVTests		= 0;
	mNbPrimPrimTests	= 0;
	mNbBVPrimTests		= 0;
	mPairs.Reset();

	// Setup matrices
	Matrix4x4 InvWorld0, InvWorld1;
	if(world0)	InvertPRMatrix(InvWorld0, *world0);
	else		InvWorld0.Identity();

	if(world1)	InvertPRMatrix(InvWorld1, *world1);
	else		InvWorld1.Identity();

	Matrix4x4 World0to1 = world0 ? (*world0 * InvWorld1) : InvWorld1;
	Matrix4x4 World1to0 = world1 ? (*world1 * InvWorld0) : InvWorld0;

	mR0to1 = World0to1;		World0to1.GetTrans(mT0to1);
	mR1to0 = World1to0;		World1to0.GetTrans(mT1to0);

	// Precompute absolute 1-to-0 rotation matrix
	for(udword i=0;i<3;i++)
	{
		for(udword j=0;j<3;j++)
		{
			// Epsilon value prevents floating-point inaccuracies (strategy borrowed from RAPID)
			mAR.m[i][j] = 1e-6f + fabsf(mR1to0.m[i][j]);
		}
	}
}


/**
* Matrix4x4 World0to1 = world0 ? (*world0 * InvWorld1) : InvWorld1;
* We must translate the point in world0 into the abosolute framework that is why world0 go first
* Then we should transform the point' relative to the world1 
* That is the inverse transfrom of the word1 so that is Inverse World1
*/


