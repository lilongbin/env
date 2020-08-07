#ifndef HSM_ENGINE_CFG_H
#   define HSM_ENGINE_CFG_H
/*===========================================================================*/
/**
 * @file hsm_engine_cfg.h
 *
 *   HSM Engine implementation configuration.
 *
 * %full_filespec:hsm_engine_cfg.h~1:incl:ctc_ec#77 %
 * @version %version:1 %
 * @author  %derived_by:fzy8g9 %
 * @date    %date_modified:Wed May  4 16:15:50 2016 %
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
 *   Defines program-specific configuration values for the implementation of
 *   the HSM engine.
 *
 * @section ABBR ABBREVIATIONS:
 *   - HSM = hierarchical state machine
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *     - SDD_10016705_State_Machine_Engine.doc
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
 * @addtogroup hsm_engine
 * @{
 */
/*==========================================================================*/

/*===========================================================================*
 * Header Files
 *===========================================================================*/

/*===========================================================================*
 * #define Constants
 *===========================================================================*/

/**
 * Defines the maximum number of levels that states can be nested.
 *
 * @note This value applies to all statecharts in the system and therefore
 *       must be chosen based on the worst-case need within the system's
 *       statecharts.
 */
#   define HSM_MAX_NESTING_LEVELS 20

/*===========================================================================*
 * #define MACROS
 *===========================================================================*/

/*===========================================================================*
 * Custom Type Declarations
 *===========================================================================*/

/*===========================================================================*/
/*!
 * @file hsm_engine_cfg.h
 *
 * @section RH REVISION HISTORY (top to bottom: last revision to first revision)
 *
 * - 07-aug-2008 kirk bailey
 *   -Created initial file.
 *
 */
/*===========================================================================*/
/** @} doxygen end group */
#endif /* HSM_ENGINE_CFG_H */
