#ifndef HSM_CHECK_H
#   define HSM_CHECK_H
/*===========================================================================*/
/**
 * @file hsm_check.h
 *
 * Defines the local interface to the logic that checks a statechart to
 * insure that it is well-formed.
 *
 * %full_filespec:hsm_check.h~kok_basa#6:incl:kok_aud#1 %
 * @version %version:kok_basa#6 %
 * @author  %derived_by:dzq92s %
 * @date    %date_modified:Sun Jul 25 14:34:02 2010 %
 *
 *------------------------------------------------------------------------------
 *
 * Copyright 2007 Technologies, Inc., All Rights Reserved.
 * Confidential
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *
 *   This header file defines a function that performs a run-time analysis
 *   of a statechart definition, looking for errors. Only a small subset of
 *   error conditions can be checked at compile time; the remainder must
 *   be performed at run time. Once a given statechart definition passes
 *   the run time test, it need not be tested again (until the statechart
 *   definition changes). *
 *
 * @section ABBR ABBREVIATIONS:
 *   - HSM = hierarchical state machine
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *     - SDD_10016705_State_Machine_Check.doc
 *
 *   - Requirements Document(s):
 *     - SRS_10016705_State_Machine.doc
 *
 *   - Applicable Standards (in order of precedence: highest first):
 *     - <a href="http://kogespw1.delcoelect.com:8508/passthru/consumer?name=SW%20REF%20264.15D&doctype=K8DBSDoc">
 *       SW REF 264.15D "C Coding Standards" [12-Mar-2006]</a>
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
 *   - None
 *
 * @ingroup hsm_engine
 * @{
 */
/*==========================================================================*/

/*===========================================================================*
 * Header Files
 *===========================================================================*/
#   include "hsm_engine.h"

/*===========================================================================*
 * Exported Preprocessor #define Constants
 *===========================================================================*/

/*===========================================================================*
 * Exported Preprocessor #define MACROS
 *===========================================================================*/

/*===========================================================================*
 * Exported Type Declarations
 *===========================================================================*/

/*===========================================================================*
 * Exported Const Object Declarations
 *===========================================================================*/

/*===========================================================================*
 * Exported Function Prototypes
 *===========================================================================*/

/**
 * Checks the statechart definition to make sure it is well-formed (legal).
 * Illegal statechart definitions trigger Error Management (EM) asserts,
 * which would typically (depending on system configuration) mean that
 * the function would not return.
 *
 * @param [in] state_defn The list and number of states in the statechart.
 */
void HSM_Check_Statechart_Defn(HSM_State_Defn_T const *state_defn);

/*===========================================================================*
 * Exported Inline Function Definitions and #define Function-Like Macros
 *===========================================================================*/

/*===========================================================================*/
/*!
 * @file hsm_check.h
 *
 * @section RH REVISION HISTORY (top to bottom: last revision to first revision)
 *
 * - 25-Jul-2010 Kirk Bailey Rev 6
 *   - Replaced "bool" with "bool_t".
 *
 * - 21-feb-2008 kirk bailey
 *   - Moved hsm_Check_Statechart_Defn to public API.
 *
 * - 15-nov-2007 kirk bailey
 *   - Changed preconditions for Check_Is_Ancestor_State to support HSM_TOP.
 *   - Converted to new Doyxgen format.
 *
 * - 04-apr-2007 kirk bailey
 *   -Created file.
 */
/*===========================================================================*/
/** @} doxygen end group */
#endif /* HSM_CHECK_H */
