

#include "CodeLibrary.hpp"
#include "Utility/Sort.hpp"

VOID Bolitho::Swap(LPVOID pA, LPVOID pB, SIZE_T Length)
{
  LPBYTE pAA = (LPBYTE)pA;
  LPBYTE pBB = (LPBYTE)pB;

  for (SIZE_T i=0; i<Length; i++)
  { BYTE Temp = pAA[i]; pAA[i] = pBB[i]; pBB[i] = Temp; }
}
