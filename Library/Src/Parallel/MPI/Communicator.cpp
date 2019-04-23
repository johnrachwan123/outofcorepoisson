/***************************************************************************************************
 * File:         $URL: http://svn.bolitho.us/Library/Trunk/Src/IO/Compression/Zip.cpp $
 * Author:       $Author: BOLITHO\matthew $
 * Revision:     $Rev: 1649 $
 * Last Updated: $Date: 2008-10-29 09:04:16 -0400 (Wed, 29 Oct 2008) $
 * 
 * 
 * Copyright (c) 2004-2007, Matthew G Bolitho
 * All rights reserved.
 *
 *
 * Microsoft Reference License (Ms-RL)
 *   
 * This license governs use of the accompanying software. If you use the software, you accept 
 * this license. If you do not accept the license, do not use the software.
 *
 * 1. Definitions
 *  - The terms "reproduce," "reproduction" and "distribution" have the same meaning here as under 
 *    U.S. copyright law.
 *  - "You" means the licensee of the software.
 *  - "Your company" means the company you worked for when you downloaded the software.
 *  - "Reference use" means use of the software within your company as a reference, in read only 
 *    form, for the sole purposes of debugging your products, maintaining your products, or 
 *    enhancing the interoperability of your products with the software, and specifically excludes 
 *    the right to distribute the software outside of your company.
 *  - "Licensed patents" means any Licensor patent claims which read directly on the software as 
 *    distributed by the Licensor under this license.
 * 
 * 2. Grant of Rights
 *  (A) Copyright Grant- Subject to the terms of this license, the Licensor grants you a non-transferable, 
 *      non-exclusive, worldwide, royalty-free copyright license to reproduce the software for reference use.
 *  (B) Patent Grant- Subject to the terms of this license, the Licensor grants you a non-transferable,
 *      non-exclusive, worldwide, royalty-free patent license under licensed patents for reference use.
 * 
 * 3. Limitations
 *  (A) No Trademark License - This license does not grant you any rights to use the Licensor's name
 *      logo, or trademarks.
 *  (B) If you begin patent litigation against the Licensor over patents that you think may apply 
 *      to the software (including a cross-claim or counterclaim in a lawsuit), your license to the 
 *      software ends automatically.
 *  (C) The software is licensed "as-is." You bear the risk of using it. The Licensor gives no express 
 *      warranties, guarantees or conditions. You may have additional consumer rights under your local 
 *      laws which this license cannot change. To the extent permitted under your local laws, the 
 *      Licensor excludes the implied warranties of merchantability, fitness for a particular purpose 
 *      and non-infringement.
 *
 ***************************************************************************************************/

#include "CodeLibrary.hpp"
#include "Parallel/MPI/MPI.hpp"
#include "Parallel/MPI/Communicator.hpp"

using namespace Bolitho;
using namespace Bolitho::Parallel::MessagePassingInterface;

//---------------------------------------------------------------------------------------------------
MPI_Datatype TranslateDataType(DWORD Type)
{
  if (Type == DataType::BYTE)
    return MPI_BYTE;
}
//---------------------------------------------------------------------------------------------------
Communicator::Communicator(MPI_Comm Communicator)
{
  m_Communicator = Communicator;
}
//---------------------------------------------------------------------------------------------------
UINT Communicator::GetRank()
{
  UINT Rank;
  MPI_Comm_rank(m_Communicator, (int*)&Rank);

  return Rank;
}
//---------------------------------------------------------------------------------------------------
UINT Communicator::GetSize()
{
  UINT Size;
  MPI_Comm_size(m_Communicator, (int*)&Size);

  return Size;
}
//---------------------------------------------------------------------------------------------------
VOID Communicator::Barrier()
{
  MPI_Barrier(m_Communicator);
}
//---------------------------------------------------------------------------------------------------
VOID Communicator::Send(LPCVOID pData, SIZE_T Length, DWORD DataType, UINT Destination, UINT Tag)
{
  MPI_Send((void**)pData, (int)Length, TranslateDataType(DataType), Destination, Tag, m_Communicator);
}
//---------------------------------------------------------------------------------------------------
SIZE_T Communicator::Recieve(LPVOID pData, SIZE_T MaximumLength, DWORD DataType, UINT Source, UINT Tag)
{
  MPI_Status Status;
  return MPI_Recv(pData, (int)MaximumLength, TranslateDataType(DataType), Source, Tag, m_Communicator, &Status);
}
//---------------------------------------------------------------------------------------------------
