/** @file
 * Copyright (c) 2016-2019,2021 Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0

 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

#include "val/include/bsa_acs_val.h"
#include "val/include/bsa_acs_pe.h"

#define TEST_NUM   (ACS_PE_TEST_NUM_BASE  +  7)
#define TEST_DESC  "B_PE_07:  Check Little Endian support      "

static
void
payload()
{
  uint64_t data = 0;
  uint32_t index = val_pe_get_index_mpid(val_pe_get_mpid());

  /* Check the current endianness setting of SCTLR.EE */
  if (val_pe_reg_read(CurrentEL) == AARCH64_EL2) {
      data = val_pe_reg_read(SCTLR_EL2);
  } else if (val_pe_reg_read(CurrentEL) == AARCH64_EL1) {
      data = val_pe_reg_read(SCTLR_EL1);
  }

  if (((data >> 25) & 1) == 0) //Bit 25 must be 0
      val_set_status(index, RESULT_PASS(TEST_NUM, 02));
  else
      val_set_status(index, RESULT_FAIL(TEST_NUM, 02));

  return;
}

uint32_t
os_c007_entry(uint32_t num_pe)
{
  uint32_t status = ACS_STATUS_FAIL;

  status = val_initialize_test(TEST_NUM, TEST_DESC, num_pe);
  if (status != ACS_STATUS_SKIP)
      val_run_test_payload(TEST_NUM, num_pe, payload, 0);

  /* get the result from all PE and check for failure */
  status = val_check_for_error(TEST_NUM, num_pe);

  val_report_status(0, BSA_ACS_END(TEST_NUM));

  return status;
}
