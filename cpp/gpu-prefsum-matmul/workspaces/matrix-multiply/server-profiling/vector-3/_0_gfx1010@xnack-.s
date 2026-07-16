	.text
	.amdgcn_target "amdgcn-amd-amdhsa--gfx1010:xnack-"
	.amdhsa_code_object_version 5
	.protected	mul                     ; -- Begin function mul
	.globl	mul
	.p2align	8
	.type	mul,@function
mul:                                    ; @mul
; %bb.0:
	s_clause 0x1
	s_load_dword s16, s[4:5], 0x20
	s_load_dwordx8 s[8:15], s[4:5], 0x0
	s_lshl_b32 s2, s7, 4
	v_mov_b32_e32 v89, 0
	v_mov_b32_e32 v88, 0
	v_mov_b32_e32 v87, 0
	v_mov_b32_e32 v86, 0
	v_mov_b32_e32 v85, 0
	v_mov_b32_e32 v84, 0
	v_mov_b32_e32 v83, 0
	v_mov_b32_e32 v82, 0
	v_mov_b32_e32 v81, 0
	v_mov_b32_e32 v80, 0
	v_mov_b32_e32 v79, 0
	v_mov_b32_e32 v78, 0
	v_mov_b32_e32 v77, 0
	v_mov_b32_e32 v76, 0
	v_mov_b32_e32 v75, 0
	v_mov_b32_e32 v74, 0
	v_mov_b32_e32 v73, 0
	s_waitcnt lgkmcnt(0)
	s_mul_hi_u32 s0, s16, 0xaaaaaaab
	s_sub_i32 s1, 0, s16
	s_lshr_b32 s0, s0, 2
	s_lshr_b32 s15, s15, 1
	s_mul_i32 s3, s0, -6
	v_mov_b32_e32 v72, 0
	s_cmp_lg_u32 s3, s1
	v_mov_b32_e32 v71, 0
	s_cselect_b32 s1, -1, 0
	v_mov_b32_e32 v70, 0
	s_cmp_lg_u32 s1, 0
	v_mov_b32_e32 v69, 0
	v_mov_b32_e32 v68, 0
	v_mov_b32_e32 v67, 0
	v_mov_b32_e32 v66, 0
	v_mov_b32_e32 v65, 0
	v_mov_b32_e32 v64, 0
	v_mov_b32_e32 v63, 0
	v_mov_b32_e32 v62, 0
	v_mov_b32_e32 v61, 0
	v_mov_b32_e32 v60, 0
	v_mov_b32_e32 v59, 0
	v_mov_b32_e32 v58, 0
	v_mov_b32_e32 v57, 0
	v_mov_b32_e32 v56, 0
	v_mov_b32_e32 v55, 0
	v_mov_b32_e32 v54, 0
	v_mov_b32_e32 v53, 0
	v_mov_b32_e32 v52, 0
	v_mov_b32_e32 v51, 0
	v_mov_b32_e32 v50, 0
	v_mov_b32_e32 v49, 0
	v_mov_b32_e32 v48, 0
	v_mov_b32_e32 v47, 0
	v_mov_b32_e32 v46, 0
	v_mov_b32_e32 v45, 0
	v_mov_b32_e32 v44, 0
	v_mov_b32_e32 v43, 0
	v_mov_b32_e32 v42, 0
	v_lshlrev_b32_e32 v92, 3, v1
	v_mul_u32_u24_e32 v93, 3, v0
	s_addc_u32 s17, s0, 0
	s_mul_i32 s3, s6, 48
	s_cmp_eq_u32 s17, 0
	s_cbranch_scc1 .LBB0_50
; %bb.1:
	s_load_dword s0, s[4:5], 0x34
	s_lshr_b32 s4, s16, 1
	s_add_i32 s1, s2, 1
	v_mov_b32_e32 v42, 0
	s_mul_i32 s18, s4, s1
	v_mov_b32_e32 v43, 0
	v_mov_b32_e32 v44, 0
	v_mov_b32_e32 v45, 0
	v_mov_b32_e32 v46, 0
	v_mov_b32_e32 v47, 0
	v_mov_b32_e32 v48, 0
	v_mov_b32_e32 v49, 0
	v_mov_b32_e32 v50, 0
	v_mov_b32_e32 v51, 0
	v_mov_b32_e32 v52, 0
	v_mov_b32_e32 v53, 0
	v_mov_b32_e32 v54, 0
	v_mov_b32_e32 v55, 0
	v_mov_b32_e32 v56, 0
	v_mov_b32_e32 v57, 0
	s_waitcnt lgkmcnt(0)
	s_and_b32 s1, s0, 0xffff
	s_lshr_b32 s24, s0, 16
	v_mad_u32_u24 v94, v1, s1, v0
	v_mov_b32_e32 v58, 0
	v_mov_b32_e32 v59, 0
	v_mov_b32_e32 v60, 0
	v_mov_b32_e32 v61, 0
	v_lshlrev_b32_e32 v95, 3, v94
	v_cmp_gt_u32_e64 s0, 48, v94
	v_mov_b32_e32 v62, 0
	v_mov_b32_e32 v63, 0
	v_mov_b32_e32 v64, 0
	v_mov_b32_e32 v65, 0
	v_mov_b32_e32 v66, 0
	v_mov_b32_e32 v67, 0
	v_mov_b32_e32 v68, 0
	v_mov_b32_e32 v69, 0
	v_mov_b32_e32 v70, 0
	v_mov_b32_e32 v71, 0
	v_mov_b32_e32 v72, 0
	v_mov_b32_e32 v73, 0
	v_mov_b32_e32 v74, 0
	v_mov_b32_e32 v75, 0
	v_mov_b32_e32 v76, 0
	v_mov_b32_e32 v77, 0
	v_mov_b32_e32 v78, 0
	v_mov_b32_e32 v79, 0
	v_mov_b32_e32 v80, 0
	v_mov_b32_e32 v81, 0
	v_mov_b32_e32 v82, 0
	v_mov_b32_e32 v83, 0
	v_mov_b32_e32 v84, 0
	v_mov_b32_e32 v85, 0
	v_mov_b32_e32 v86, 0
	v_mov_b32_e32 v87, 0
	v_mov_b32_e32 v88, 0
	v_mov_b32_e32 v89, 0
	v_lshlrev_b32_e32 v96, 3, v93
	v_mad_u32_u24 v97, v92, 24, 0x900
	v_add_nc_u32_e32 v98, 0x900, v95
	v_add_nc_u32_e32 v99, 0x918, v95
	v_add_nc_u32_e32 v100, 0x930, v95
	v_add_nc_u32_e32 v101, 0x948, v95
	v_add_nc_u32_e32 v102, 0x180, v95
	v_add_nc_u32_e32 v103, 0x300, v95
	v_add_nc_u32_e32 v104, 0x480, v95
	v_add_nc_u32_e32 v105, 0x600, v95
	v_add_nc_u32_e32 v106, 0x780, v95
	v_mov_b32_e32 v91, 0
	s_add_i32 s19, s2, 2
	s_add_i32 s20, s2, 3
	s_lshl_b32 s21, s15, 1
	s_mul_i32 s22, s15, 3
	s_lshl_b32 s23, s15, 2
	s_mul_i32 s27, s15, 5
	s_mul_i32 s25, s7, s4
	s_mul_i32 s24, s24, s1
	s_mul_i32 s5, s15, 6
	s_add_i32 s6, s15, s3
	s_lshl_b32 s7, s4, 2
	s_mul_i32 s19, s4, s19
	s_mul_i32 s20, s4, s20
	s_add_i32 s21, s3, s21
	s_add_i32 s22, s3, s22
	s_add_i32 s23, s3, s23
	s_lshl_b32 s25, s25, 4
	s_lshl_b32 s26, s24, 3
	s_add_i32 s27, s3, s27
	s_mov_b32 s28, 0
	s_mov_b32 s29, s3
	s_mov_b32 s30, 0
	v_cmp_gt_u32_e32 vcc_lo, 3, v94
	s_branch .LBB0_3
.LBB0_2:                                ; %Flow585
                                        ;   in Loop: Header=BB0_3 Depth=1
	s_or_b32 exec_lo, exec_lo, s31
	s_waitcnt lgkmcnt(0)
	s_barrier
	ds_read2_b64 v[16:19], v96 offset1:1
	ds_read2_b64 v[24:27], v97 offset1:1
	ds_read2_b64 v[0:3], v97 offset0:2 offset1:3
	ds_read2_b64 v[28:31], v97 offset0:6 offset1:7
	ds_read2_b64 v[12:15], v97 offset0:8 offset1:9
	ds_read2_b64 v[32:35], v97 offset0:12 offset1:13
	ds_read2_b64 v[8:11], v97 offset0:14 offset1:15
	ds_read2_b64 v[36:39], v97 offset0:18 offset1:19
	ds_read2_b64 v[4:7], v97 offset0:20 offset1:21
	s_add_i32 s30, s30, 1
	s_add_i32 s25, s25, 3
	s_add_i32 s28, s28, 3
	s_add_i32 s18, s18, 3
	s_add_i32 s19, s19, 3
	s_add_i32 s20, s20, 3
	s_add_i32 s29, s29, s5
	s_add_i32 s6, s6, s5
	s_add_i32 s21, s21, s5
	s_add_i32 s22, s22, s5
	s_add_i32 s23, s23, s5
	s_add_i32 s27, s27, s5
	s_waitcnt lgkmcnt(7)
	v_fmac_f32_e32 v89, v24, v17
	v_fmac_f32_e32 v88, v24, v16
	v_fmac_f32_e32 v87, v24, v19
	v_fmac_f32_e32 v86, v24, v18
	s_waitcnt lgkmcnt(6)
	v_fmac_f32_e32 v83, v2, v17
	v_fmac_f32_e32 v82, v2, v16
	v_fmac_f32_e32 v81, v2, v19
	v_fmac_f32_e32 v80, v2, v18
	s_waitcnt lgkmcnt(5)
	v_fmac_f32_e32 v77, v28, v17
	v_fmac_f32_e32 v76, v28, v16
	v_fmac_f32_e32 v75, v28, v19
	v_fmac_f32_e32 v74, v28, v18
	s_waitcnt lgkmcnt(4)
	v_fmac_f32_e32 v71, v14, v17
	v_fmac_f32_e32 v70, v14, v16
	v_fmac_f32_e32 v69, v14, v19
	v_fmac_f32_e32 v68, v14, v18
	s_waitcnt lgkmcnt(3)
	v_fmac_f32_e32 v65, v32, v17
	v_fmac_f32_e32 v64, v32, v16
	v_fmac_f32_e32 v63, v32, v19
	v_fmac_f32_e32 v62, v32, v18
	s_waitcnt lgkmcnt(2)
	v_fmac_f32_e32 v59, v10, v17
	v_fmac_f32_e32 v58, v10, v16
	v_fmac_f32_e32 v57, v10, v19
	v_fmac_f32_e32 v56, v10, v18
	s_waitcnt lgkmcnt(1)
	v_fmac_f32_e32 v53, v36, v17
	v_fmac_f32_e32 v52, v36, v16
	v_fmac_f32_e32 v51, v36, v19
	v_fmac_f32_e32 v50, v36, v18
	s_waitcnt lgkmcnt(0)
	v_fmac_f32_e32 v47, v6, v17
	v_fmac_f32_e32 v46, v6, v16
	v_fmac_f32_e32 v45, v6, v19
	v_fmac_f32_e32 v44, v6, v18
	ds_read2_b64 v[16:19], v96 offset0:2 offset1:48
	s_cmp_eq_u32 s30, s17
	s_waitcnt lgkmcnt(0)
	v_fmac_f32_e32 v85, v24, v17
	v_fmac_f32_e32 v84, v24, v16
	v_fmac_f32_e32 v88, v25, v18
	v_fmac_f32_e32 v89, v25, v19
	v_fmac_f32_e32 v79, v2, v17
	v_fmac_f32_e32 v78, v2, v16
	v_fmac_f32_e32 v82, v3, v18
	v_fmac_f32_e32 v83, v3, v19
	v_fmac_f32_e32 v73, v28, v17
	v_fmac_f32_e32 v72, v28, v16
	v_fmac_f32_e32 v76, v29, v18
	v_fmac_f32_e32 v77, v29, v19
	v_fmac_f32_e32 v67, v14, v17
	v_fmac_f32_e32 v66, v14, v16
	v_fmac_f32_e32 v70, v15, v18
	v_fmac_f32_e32 v71, v15, v19
	v_fmac_f32_e32 v61, v32, v17
	v_fmac_f32_e32 v60, v32, v16
	v_fmac_f32_e32 v64, v33, v18
	v_fmac_f32_e32 v65, v33, v19
	v_fmac_f32_e32 v55, v10, v17
	v_fmac_f32_e32 v54, v10, v16
	v_fmac_f32_e32 v58, v11, v18
	v_fmac_f32_e32 v59, v11, v19
	v_fmac_f32_e32 v49, v36, v17
	v_fmac_f32_e32 v48, v36, v16
	v_fmac_f32_e32 v52, v37, v18
	v_fmac_f32_e32 v53, v37, v19
	v_fmac_f32_e32 v43, v6, v17
	v_fmac_f32_e32 v42, v6, v16
	v_fmac_f32_e32 v46, v7, v18
	v_fmac_f32_e32 v47, v7, v19
	ds_read2_b64 v[16:19], v96 offset0:49 offset1:50
	s_waitcnt lgkmcnt(0)
	v_fmac_f32_e32 v86, v25, v16
	v_fmac_f32_e32 v87, v25, v17
	v_fmac_f32_e32 v84, v25, v18
	v_fmac_f32_e32 v85, v25, v19
	v_fmac_f32_e32 v78, v3, v18
	v_fmac_f32_e32 v79, v3, v19
	v_fmac_f32_e32 v72, v29, v18
	v_fmac_f32_e32 v73, v29, v19
	v_fmac_f32_e32 v66, v15, v18
	v_fmac_f32_e32 v67, v15, v19
	v_fmac_f32_e32 v60, v33, v18
	v_fmac_f32_e32 v61, v33, v19
	v_fmac_f32_e32 v54, v11, v18
	v_fmac_f32_e32 v55, v11, v19
	v_fmac_f32_e32 v48, v37, v18
	v_fmac_f32_e32 v49, v37, v19
	v_fmac_f32_e32 v42, v7, v18
	v_fmac_f32_e32 v43, v7, v19
	ds_read2_b64 v[22:25], v96 offset0:96 offset1:97
	ds_read2_b64 v[18:21], v96 offset0:98 offset1:144
	v_fmac_f32_e32 v80, v3, v16
	v_fmac_f32_e32 v81, v3, v17
	v_fmac_f32_e32 v74, v29, v16
	v_fmac_f32_e32 v75, v29, v17
	v_fmac_f32_e32 v68, v15, v16
	v_fmac_f32_e32 v69, v15, v17
	v_fmac_f32_e32 v62, v33, v16
	v_fmac_f32_e32 v63, v33, v17
	v_fmac_f32_e32 v56, v11, v16
	v_fmac_f32_e32 v57, v11, v17
	v_fmac_f32_e32 v50, v37, v16
	v_fmac_f32_e32 v51, v37, v17
	v_fmac_f32_e32 v44, v7, v16
	v_fmac_f32_e32 v45, v7, v17
	ds_read2_b64 v[14:17], v96 offset0:145 offset1:146
	s_waitcnt lgkmcnt(2)
	v_fmac_f32_e32 v89, v26, v23
	v_fmac_f32_e32 v88, v26, v22
	v_fmac_f32_e32 v87, v26, v25
	v_fmac_f32_e32 v86, v26, v24
	s_waitcnt lgkmcnt(1)
	v_fmac_f32_e32 v85, v26, v19
	v_fmac_f32_e32 v84, v26, v18
	v_fmac_f32_e32 v77, v30, v23
	v_fmac_f32_e32 v76, v30, v22
	v_fmac_f32_e32 v75, v30, v25
	v_fmac_f32_e32 v74, v30, v24
	v_fmac_f32_e32 v73, v30, v19
	v_fmac_f32_e32 v72, v30, v18
	v_fmac_f32_e32 v65, v34, v23
	v_fmac_f32_e32 v64, v34, v22
	v_fmac_f32_e32 v63, v34, v25
	v_fmac_f32_e32 v62, v34, v24
	v_fmac_f32_e32 v61, v34, v19
	v_fmac_f32_e32 v60, v34, v18
	v_fmac_f32_e32 v53, v38, v23
	v_fmac_f32_e32 v52, v38, v22
	v_fmac_f32_e32 v51, v38, v25
	v_fmac_f32_e32 v50, v38, v24
	v_fmac_f32_e32 v49, v38, v19
	v_fmac_f32_e32 v48, v38, v18
	v_fmac_f32_e32 v88, v27, v20
	v_fmac_f32_e32 v89, v27, v21
	s_waitcnt lgkmcnt(0)
	v_fmac_f32_e32 v86, v27, v14
	v_fmac_f32_e32 v87, v27, v15
	v_fmac_f32_e32 v84, v27, v16
	v_fmac_f32_e32 v85, v27, v17
	v_fmac_f32_e32 v76, v31, v20
	v_fmac_f32_e32 v77, v31, v21
	v_fmac_f32_e32 v74, v31, v14
	v_fmac_f32_e32 v75, v31, v15
	v_fmac_f32_e32 v72, v31, v16
	v_fmac_f32_e32 v73, v31, v17
	v_fmac_f32_e32 v64, v35, v20
	v_fmac_f32_e32 v65, v35, v21
	v_fmac_f32_e32 v62, v35, v14
	v_fmac_f32_e32 v63, v35, v15
	v_fmac_f32_e32 v60, v35, v16
	v_fmac_f32_e32 v61, v35, v17
	v_fmac_f32_e32 v52, v39, v20
	v_fmac_f32_e32 v53, v39, v21
	v_fmac_f32_e32 v50, v39, v14
	v_fmac_f32_e32 v51, v39, v15
	v_fmac_f32_e32 v48, v39, v16
	v_fmac_f32_e32 v49, v39, v17
	ds_read2_b64 v[38:41], v97 offset0:4 offset1:5
	ds_read2_b64 v[34:37], v97 offset0:10 offset1:11
	ds_read2_b64 v[26:29], v97 offset0:16 offset1:17
	ds_read2_b64 v[30:33], v97 offset0:22 offset1:23
	s_waitcnt lgkmcnt(3)
	v_fmac_f32_e32 v83, v38, v23
	v_fmac_f32_e32 v82, v38, v22
	v_fmac_f32_e32 v81, v38, v25
	v_fmac_f32_e32 v80, v38, v24
	s_waitcnt lgkmcnt(2)
	v_fmac_f32_e32 v71, v34, v23
	v_fmac_f32_e32 v70, v34, v22
	v_fmac_f32_e32 v69, v34, v25
	v_fmac_f32_e32 v68, v34, v24
	s_waitcnt lgkmcnt(1)
	v_fmac_f32_e32 v59, v26, v23
	v_fmac_f32_e32 v58, v26, v22
	v_fmac_f32_e32 v57, v26, v25
	v_fmac_f32_e32 v56, v26, v24
	s_waitcnt lgkmcnt(0)
	v_fmac_f32_e32 v47, v30, v23
	v_fmac_f32_e32 v46, v30, v22
	v_fmac_f32_e32 v45, v30, v25
	v_fmac_f32_e32 v44, v30, v24
	v_fmac_f32_e32 v79, v38, v19
	v_fmac_f32_e32 v78, v38, v18
	v_fmac_f32_e32 v67, v34, v19
	v_fmac_f32_e32 v66, v34, v18
	v_fmac_f32_e32 v55, v26, v19
	v_fmac_f32_e32 v54, v26, v18
	v_fmac_f32_e32 v43, v30, v19
	v_fmac_f32_e32 v42, v30, v18
	v_fmac_f32_e32 v82, v39, v20
	v_fmac_f32_e32 v83, v39, v21
	v_fmac_f32_e32 v70, v35, v20
	v_fmac_f32_e32 v71, v35, v21
	v_fmac_f32_e32 v58, v27, v20
	v_fmac_f32_e32 v59, v27, v21
	v_fmac_f32_e32 v46, v31, v20
	v_fmac_f32_e32 v47, v31, v21
	v_fmac_f32_e32 v80, v39, v14
	v_fmac_f32_e32 v81, v39, v15
	v_fmac_f32_e32 v78, v39, v16
	v_fmac_f32_e32 v79, v39, v17
	v_fmac_f32_e32 v68, v35, v14
	v_fmac_f32_e32 v69, v35, v15
	v_fmac_f32_e32 v66, v35, v16
	v_fmac_f32_e32 v67, v35, v17
	v_fmac_f32_e32 v56, v27, v14
	v_fmac_f32_e32 v57, v27, v15
	v_fmac_f32_e32 v54, v27, v16
	v_fmac_f32_e32 v55, v27, v17
	v_fmac_f32_e32 v44, v31, v14
	v_fmac_f32_e32 v45, v31, v15
	v_fmac_f32_e32 v42, v31, v16
	v_fmac_f32_e32 v43, v31, v17
	ds_read2_b64 v[14:17], v96 offset0:192 offset1:193
	ds_read2_b64 v[22:25], v96 offset0:241 offset1:242
	ds_read2_b64 v[18:21], v96 offset0:194 offset1:240
	s_waitcnt lgkmcnt(0)
	s_barrier
	v_fmac_f32_e32 v89, v0, v15
	v_fmac_f32_e32 v88, v0, v14
	v_fmac_f32_e32 v87, v0, v17
	v_fmac_f32_e32 v86, v0, v16
	v_fmac_f32_e32 v85, v0, v19
	v_fmac_f32_e32 v84, v0, v18
	v_fmac_f32_e32 v83, v40, v15
	v_fmac_f32_e32 v82, v40, v14
	v_fmac_f32_e32 v81, v40, v17
	v_fmac_f32_e32 v80, v40, v16
	v_fmac_f32_e32 v79, v40, v19
	v_fmac_f32_e32 v78, v40, v18
	v_fmac_f32_e32 v77, v12, v15
	v_fmac_f32_e32 v76, v12, v14
	v_fmac_f32_e32 v75, v12, v17
	v_fmac_f32_e32 v74, v12, v16
	v_fmac_f32_e32 v73, v12, v19
	v_fmac_f32_e32 v72, v12, v18
	v_fmac_f32_e32 v71, v36, v15
	v_fmac_f32_e32 v70, v36, v14
	v_fmac_f32_e32 v69, v36, v17
	v_fmac_f32_e32 v68, v36, v16
	v_fmac_f32_e32 v67, v36, v19
	v_fmac_f32_e32 v66, v36, v18
	v_fmac_f32_e32 v65, v8, v15
	v_fmac_f32_e32 v64, v8, v14
	v_fmac_f32_e32 v63, v8, v17
	v_fmac_f32_e32 v62, v8, v16
	v_fmac_f32_e32 v61, v8, v19
	v_fmac_f32_e32 v60, v8, v18
	v_fmac_f32_e32 v59, v28, v15
	v_fmac_f32_e32 v58, v28, v14
	v_fmac_f32_e32 v57, v28, v17
	v_fmac_f32_e32 v56, v28, v16
	v_fmac_f32_e32 v55, v28, v19
	v_fmac_f32_e32 v54, v28, v18
	v_fmac_f32_e32 v53, v4, v15
	v_fmac_f32_e32 v52, v4, v14
	v_fmac_f32_e32 v51, v4, v17
	v_fmac_f32_e32 v50, v4, v16
	v_fmac_f32_e32 v49, v4, v19
	v_fmac_f32_e32 v48, v4, v18
	v_fmac_f32_e32 v47, v32, v15
	v_fmac_f32_e32 v46, v32, v14
	v_fmac_f32_e32 v45, v32, v17
	v_fmac_f32_e32 v44, v32, v16
	v_fmac_f32_e32 v43, v32, v19
	v_fmac_f32_e32 v42, v32, v18
	v_fmac_f32_e32 v88, v1, v20
	v_fmac_f32_e32 v89, v1, v21
	v_fmac_f32_e32 v86, v1, v22
	v_fmac_f32_e32 v87, v1, v23
	v_fmac_f32_e32 v84, v1, v24
	v_fmac_f32_e32 v85, v1, v25
	v_fmac_f32_e32 v82, v41, v20
	v_fmac_f32_e32 v83, v41, v21
	v_fmac_f32_e32 v80, v41, v22
	v_fmac_f32_e32 v81, v41, v23
	v_fmac_f32_e32 v78, v41, v24
	v_fmac_f32_e32 v79, v41, v25
	v_fmac_f32_e32 v76, v13, v20
	v_fmac_f32_e32 v77, v13, v21
	v_fmac_f32_e32 v74, v13, v22
	v_fmac_f32_e32 v75, v13, v23
	v_fmac_f32_e32 v72, v13, v24
	v_fmac_f32_e32 v73, v13, v25
	v_fmac_f32_e32 v70, v37, v20
	v_fmac_f32_e32 v71, v37, v21
	v_fmac_f32_e32 v68, v37, v22
	v_fmac_f32_e32 v69, v37, v23
	v_fmac_f32_e32 v66, v37, v24
	v_fmac_f32_e32 v67, v37, v25
	v_fmac_f32_e32 v64, v9, v20
	v_fmac_f32_e32 v65, v9, v21
	v_fmac_f32_e32 v62, v9, v22
	v_fmac_f32_e32 v63, v9, v23
	v_fmac_f32_e32 v60, v9, v24
	v_fmac_f32_e32 v61, v9, v25
	v_fmac_f32_e32 v58, v29, v20
	v_fmac_f32_e32 v59, v29, v21
	v_fmac_f32_e32 v56, v29, v22
	v_fmac_f32_e32 v57, v29, v23
	v_fmac_f32_e32 v54, v29, v24
	v_fmac_f32_e32 v55, v29, v25
	v_fmac_f32_e32 v52, v5, v20
	v_fmac_f32_e32 v53, v5, v21
	v_fmac_f32_e32 v50, v5, v22
	v_fmac_f32_e32 v51, v5, v23
	v_fmac_f32_e32 v48, v5, v24
	v_fmac_f32_e32 v49, v5, v25
	v_fmac_f32_e32 v46, v33, v20
	v_fmac_f32_e32 v47, v33, v21
	v_fmac_f32_e32 v44, v33, v22
	v_fmac_f32_e32 v45, v33, v23
	v_fmac_f32_e32 v42, v33, v24
	v_fmac_f32_e32 v43, v33, v25
	s_cbranch_scc1 .LBB0_50
.LBB0_3:                                ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB0_5 Depth 2
                                        ;       Child Loop BB0_8 Depth 3
                                        ;       Child Loop BB0_13 Depth 3
                                        ;       Child Loop BB0_18 Depth 3
                                        ;       Child Loop BB0_23 Depth 3
                                        ;     Child Loop BB0_28 Depth 2
                                        ;     Child Loop BB0_32 Depth 2
                                        ;     Child Loop BB0_36 Depth 2
                                        ;     Child Loop BB0_40 Depth 2
                                        ;     Child Loop BB0_44 Depth 2
                                        ;     Child Loop BB0_48 Depth 2
	v_mov_b32_e32 v2, v101
	v_mov_b32_e32 v3, v100
	v_mov_b32_e32 v4, v99
	v_mov_b32_e32 v5, v98
	s_mov_b32 s31, s20
	s_mov_b32 s33, s19
	s_mov_b32 s34, s18
	s_mov_b32 s35, s25
	s_mov_b32 s36, 0
	s_branch .LBB0_5
.LBB0_4:                                ; %Flow587
                                        ;   in Loop: Header=BB0_5 Depth=2
	s_or_b32 exec_lo, exec_lo, s37
	v_add_nc_u32_e32 v5, 0x60, v5
	v_add_nc_u32_e32 v4, 0x60, v4
	v_add_nc_u32_e32 v3, 0x60, v3
	v_add_nc_u32_e32 v2, 0x60, v2
	s_add_i32 s36, s36, 4
	s_add_i32 s35, s35, s7
	s_add_i32 s34, s34, s7
	s_add_i32 s33, s33, s7
	s_add_i32 s31, s31, s7
	s_cmp_lg_u32 s36, 16
	s_cbranch_scc0 .LBB0_25
.LBB0_5:                                ;   Parent Loop BB0_3 Depth=1
                                        ; =>  This Loop Header: Depth=2
                                        ;       Child Loop BB0_8 Depth 3
                                        ;       Child Loop BB0_13 Depth 3
                                        ;       Child Loop BB0_18 Depth 3
                                        ;       Child Loop BB0_23 Depth 3
	s_and_saveexec_b32 s37, vcc_lo
	s_cbranch_execz .LBB0_10
; %bb.6:                                ;   in Loop: Header=BB0_5 Depth=2
	s_or_b32 s1, s36, s2
	v_mov_b32_e32 v6, v5
	v_mov_b32_e32 v7, v94
	s_cmp_lt_u32 s1, s14
	s_mov_b32 s38, 0
	s_cselect_b32 s39, -1, 0
	s_branch .LBB0_8
.LBB0_7:                                ;   in Loop: Header=BB0_8 Depth=3
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s40
	v_add_nc_u32_e32 v7, s24, v7
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v6, v[0:1]
	v_add_nc_u32_e32 v6, s26, v6
	v_cmp_lt_u32_e64 s1, 2, v7
	s_or_b32 s38, s1, s38
	s_andn2_b32 exec_lo, exec_lo, s38
	s_cbranch_execz .LBB0_10
.LBB0_8:                                ;   Parent Loop BB0_3 Depth=1
                                        ;     Parent Loop BB0_5 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	v_add_nc_u32_e32 v0, s28, v7
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s1, s4, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s1, s39, s1
	s_and_saveexec_b32 s40, s1
	s_cbranch_execz .LBB0_7
; %bb.9:                                ;   in Loop: Header=BB0_8 Depth=3
	v_add_nc_u32_e32 v90, s35, v7
	v_lshlrev_b64 v[0:1], 3, v[90:91]
	v_add_co_u32 v0, s1, s8, v0
	v_add_co_ci_u32_e64 v1, s1, s9, v1, s1
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_7
.LBB0_10:                               ; %Flow593
                                        ;   in Loop: Header=BB0_5 Depth=2
	s_or_b32 exec_lo, exec_lo, s37
	s_and_saveexec_b32 s37, vcc_lo
	s_cbranch_execz .LBB0_15
; %bb.11:                               ;   in Loop: Header=BB0_5 Depth=2
	s_or_b32 s1, s36, s2
	v_mov_b32_e32 v6, v4
	s_or_b32 s1, s1, 1
	v_mov_b32_e32 v7, v94
	s_cmp_lt_u32 s1, s14
	s_mov_b32 s38, 0
	s_cselect_b32 s39, -1, 0
	s_branch .LBB0_13
.LBB0_12:                               ;   in Loop: Header=BB0_13 Depth=3
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s40
	v_add_nc_u32_e32 v7, s24, v7
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v6, v[0:1]
	v_add_nc_u32_e32 v6, s26, v6
	v_cmp_lt_u32_e64 s1, 2, v7
	s_or_b32 s38, s1, s38
	s_andn2_b32 exec_lo, exec_lo, s38
	s_cbranch_execz .LBB0_15
.LBB0_13:                               ;   Parent Loop BB0_3 Depth=1
                                        ;     Parent Loop BB0_5 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	v_add_nc_u32_e32 v0, s28, v7
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s1, s4, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s1, s39, s1
	s_and_saveexec_b32 s40, s1
	s_cbranch_execz .LBB0_12
; %bb.14:                               ;   in Loop: Header=BB0_13 Depth=3
	v_add_nc_u32_e32 v90, s34, v7
	v_lshlrev_b64 v[0:1], 3, v[90:91]
	v_add_co_u32 v0, s1, s8, v0
	v_add_co_ci_u32_e64 v1, s1, s9, v1, s1
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_12
.LBB0_15:                               ; %Flow591
                                        ;   in Loop: Header=BB0_5 Depth=2
	s_or_b32 exec_lo, exec_lo, s37
	s_and_saveexec_b32 s37, vcc_lo
	s_cbranch_execz .LBB0_20
; %bb.16:                               ;   in Loop: Header=BB0_5 Depth=2
	s_or_b32 s1, s36, s2
	v_mov_b32_e32 v6, v3
	s_or_b32 s1, s1, 2
	v_mov_b32_e32 v7, v94
	s_cmp_lt_u32 s1, s14
	s_mov_b32 s38, 0
	s_cselect_b32 s39, -1, 0
	s_branch .LBB0_18
.LBB0_17:                               ;   in Loop: Header=BB0_18 Depth=3
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s40
	v_add_nc_u32_e32 v7, s24, v7
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v6, v[0:1]
	v_add_nc_u32_e32 v6, s26, v6
	v_cmp_lt_u32_e64 s1, 2, v7
	s_or_b32 s38, s1, s38
	s_andn2_b32 exec_lo, exec_lo, s38
	s_cbranch_execz .LBB0_20
.LBB0_18:                               ;   Parent Loop BB0_3 Depth=1
                                        ;     Parent Loop BB0_5 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	v_add_nc_u32_e32 v0, s28, v7
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s1, s4, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s1, s39, s1
	s_and_saveexec_b32 s40, s1
	s_cbranch_execz .LBB0_17
; %bb.19:                               ;   in Loop: Header=BB0_18 Depth=3
	v_add_nc_u32_e32 v90, s33, v7
	v_lshlrev_b64 v[0:1], 3, v[90:91]
	v_add_co_u32 v0, s1, s8, v0
	v_add_co_ci_u32_e64 v1, s1, s9, v1, s1
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_17
.LBB0_20:                               ; %Flow589
                                        ;   in Loop: Header=BB0_5 Depth=2
	s_or_b32 exec_lo, exec_lo, s37
	s_and_saveexec_b32 s37, vcc_lo
	s_cbranch_execz .LBB0_4
; %bb.21:                               ;   in Loop: Header=BB0_5 Depth=2
	s_or_b32 s1, s36, s2
	v_mov_b32_e32 v6, v2
	s_or_b32 s1, s1, 3
	v_mov_b32_e32 v7, v94
	s_cmp_lt_u32 s1, s14
	s_mov_b32 s38, 0
	s_cselect_b32 s39, -1, 0
	s_branch .LBB0_23
.LBB0_22:                               ;   in Loop: Header=BB0_23 Depth=3
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s40
	v_add_nc_u32_e32 v7, s24, v7
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v6, v[0:1]
	v_add_nc_u32_e32 v6, s26, v6
	v_cmp_lt_u32_e64 s1, 2, v7
	s_or_b32 s38, s1, s38
	s_andn2_b32 exec_lo, exec_lo, s38
	s_cbranch_execz .LBB0_4
.LBB0_23:                               ;   Parent Loop BB0_3 Depth=1
                                        ;     Parent Loop BB0_5 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	v_add_nc_u32_e32 v0, s28, v7
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s1, s4, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s1, s39, s1
	s_and_saveexec_b32 s40, s1
	s_cbranch_execz .LBB0_22
; %bb.24:                               ;   in Loop: Header=BB0_23 Depth=3
	v_add_nc_u32_e32 v90, s31, v7
	v_lshlrev_b64 v[0:1], 3, v[90:91]
	v_add_co_u32 v0, s1, s8, v0
	v_add_co_ci_u32_e64 v1, s1, s9, v1, s1
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_22
.LBB0_25:                               ;   in Loop: Header=BB0_3 Depth=1
	s_and_saveexec_b32 s31, s0
	s_cbranch_execz .LBB0_2
; %bb.26:                               ;   in Loop: Header=BB0_3 Depth=1
	s_mul_i32 s33, s30, 6
	v_mov_b32_e32 v2, v95
	v_mov_b32_e32 v3, v94
	s_cmp_lt_u32 s33, s16
	s_mov_b32 s34, 0
	s_cselect_b32 s35, -1, 0
	s_branch .LBB0_28
.LBB0_27:                               ;   in Loop: Header=BB0_28 Depth=2
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s36
	v_add_nc_u32_e32 v3, s24, v3
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v2, v[0:1]
	v_add_nc_u32_e32 v2, s26, v2
	v_cmp_lt_u32_e64 s1, 47, v3
	s_or_b32 s34, s1, s34
	s_andn2_b32 exec_lo, exec_lo, s34
	s_cbranch_execz .LBB0_30
.LBB0_28:                               ;   Parent Loop BB0_3 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_add_nc_u32_e32 v0, s3, v3
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s1, s15, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s1, s35, s1
	s_and_saveexec_b32 s36, s1
	s_cbranch_execz .LBB0_27
; %bb.29:                               ;   in Loop: Header=BB0_28 Depth=2
	v_add_nc_u32_e32 v90, s29, v3
	v_lshlrev_b64 v[0:1], 3, v[90:91]
	v_add_co_u32 v0, s1, s10, v0
	v_add_co_ci_u32_e64 v1, s1, s11, v1, s1
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_27
.LBB0_30:                               ;   in Loop: Header=BB0_3 Depth=1
	s_or_b32 exec_lo, exec_lo, s34
	s_or_b32 s1, s33, 1
	v_mov_b32_e32 v2, v102
	v_mov_b32_e32 v3, v94
	s_cmp_lt_u32 s1, s16
	s_mov_b32 s34, 0
	s_cselect_b32 s35, -1, 0
	s_branch .LBB0_32
.LBB0_31:                               ;   in Loop: Header=BB0_32 Depth=2
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s36
	v_add_nc_u32_e32 v3, s24, v3
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v2, v[0:1]
	v_add_nc_u32_e32 v2, s26, v2
	v_cmp_lt_u32_e64 s1, 47, v3
	s_or_b32 s34, s1, s34
	s_andn2_b32 exec_lo, exec_lo, s34
	s_cbranch_execz .LBB0_34
.LBB0_32:                               ;   Parent Loop BB0_3 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_add_nc_u32_e32 v0, s3, v3
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s1, s15, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s1, s35, s1
	s_and_saveexec_b32 s36, s1
	s_cbranch_execz .LBB0_31
; %bb.33:                               ;   in Loop: Header=BB0_32 Depth=2
	v_add_nc_u32_e32 v90, s6, v3
	v_lshlrev_b64 v[0:1], 3, v[90:91]
	v_add_co_u32 v0, s1, s10, v0
	v_add_co_ci_u32_e64 v1, s1, s11, v1, s1
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_31
.LBB0_34:                               ;   in Loop: Header=BB0_3 Depth=1
	s_or_b32 exec_lo, exec_lo, s34
	s_add_i32 s1, s33, 2
	v_mov_b32_e32 v2, v103
	v_mov_b32_e32 v3, v94
	s_cmp_lt_u32 s1, s16
	s_mov_b32 s34, 0
	s_cselect_b32 s35, -1, 0
	s_branch .LBB0_36
.LBB0_35:                               ;   in Loop: Header=BB0_36 Depth=2
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s36
	v_add_nc_u32_e32 v3, s24, v3
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v2, v[0:1]
	v_add_nc_u32_e32 v2, s26, v2
	v_cmp_lt_u32_e64 s1, 47, v3
	s_or_b32 s34, s1, s34
	s_andn2_b32 exec_lo, exec_lo, s34
	s_cbranch_execz .LBB0_38
.LBB0_36:                               ;   Parent Loop BB0_3 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_add_nc_u32_e32 v0, s3, v3
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s1, s15, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s1, s35, s1
	s_and_saveexec_b32 s36, s1
	s_cbranch_execz .LBB0_35
; %bb.37:                               ;   in Loop: Header=BB0_36 Depth=2
	v_add_nc_u32_e32 v90, s21, v3
	v_lshlrev_b64 v[0:1], 3, v[90:91]
	v_add_co_u32 v0, s1, s10, v0
	v_add_co_ci_u32_e64 v1, s1, s11, v1, s1
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_35
.LBB0_38:                               ;   in Loop: Header=BB0_3 Depth=1
	s_or_b32 exec_lo, exec_lo, s34
	s_add_i32 s1, s33, 3
	v_mov_b32_e32 v2, v104
	v_mov_b32_e32 v3, v94
	s_cmp_lt_u32 s1, s16
	s_mov_b32 s34, 0
	s_cselect_b32 s35, -1, 0
	s_branch .LBB0_40
.LBB0_39:                               ;   in Loop: Header=BB0_40 Depth=2
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s36
	v_add_nc_u32_e32 v3, s24, v3
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v2, v[0:1]
	v_add_nc_u32_e32 v2, s26, v2
	v_cmp_lt_u32_e64 s1, 47, v3
	s_or_b32 s34, s1, s34
	s_andn2_b32 exec_lo, exec_lo, s34
	s_cbranch_execz .LBB0_42
.LBB0_40:                               ;   Parent Loop BB0_3 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_add_nc_u32_e32 v0, s3, v3
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s1, s15, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s1, s35, s1
	s_and_saveexec_b32 s36, s1
	s_cbranch_execz .LBB0_39
; %bb.41:                               ;   in Loop: Header=BB0_40 Depth=2
	v_add_nc_u32_e32 v90, s22, v3
	v_lshlrev_b64 v[0:1], 3, v[90:91]
	v_add_co_u32 v0, s1, s10, v0
	v_add_co_ci_u32_e64 v1, s1, s11, v1, s1
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_39
.LBB0_42:                               ;   in Loop: Header=BB0_3 Depth=1
	s_or_b32 exec_lo, exec_lo, s34
	s_add_i32 s1, s33, 4
	v_mov_b32_e32 v2, v105
	v_mov_b32_e32 v3, v94
	s_cmp_lt_u32 s1, s16
	s_mov_b32 s34, 0
	s_cselect_b32 s35, -1, 0
	s_branch .LBB0_44
.LBB0_43:                               ;   in Loop: Header=BB0_44 Depth=2
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s36
	v_add_nc_u32_e32 v3, s24, v3
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v2, v[0:1]
	v_add_nc_u32_e32 v2, s26, v2
	v_cmp_lt_u32_e64 s1, 47, v3
	s_or_b32 s34, s1, s34
	s_andn2_b32 exec_lo, exec_lo, s34
	s_cbranch_execz .LBB0_46
.LBB0_44:                               ;   Parent Loop BB0_3 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_add_nc_u32_e32 v0, s3, v3
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s1, s15, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s1, s35, s1
	s_and_saveexec_b32 s36, s1
	s_cbranch_execz .LBB0_43
; %bb.45:                               ;   in Loop: Header=BB0_44 Depth=2
	v_add_nc_u32_e32 v90, s23, v3
	v_lshlrev_b64 v[0:1], 3, v[90:91]
	v_add_co_u32 v0, s1, s10, v0
	v_add_co_ci_u32_e64 v1, s1, s11, v1, s1
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_43
.LBB0_46:                               ;   in Loop: Header=BB0_3 Depth=1
	s_or_b32 exec_lo, exec_lo, s34
	s_add_i32 s33, s33, 5
	v_mov_b32_e32 v2, v106
	v_mov_b32_e32 v3, v94
	s_cmp_lt_u32 s33, s16
	s_mov_b32 s33, 0
	s_cselect_b32 s34, -1, 0
	s_branch .LBB0_48
.LBB0_47:                               ;   in Loop: Header=BB0_48 Depth=2
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s35
	v_add_nc_u32_e32 v3, s24, v3
	s_waitcnt vmcnt(0)
	s_waitcnt_vscnt null, 0x0
	ds_write_b64 v2, v[0:1]
	v_add_nc_u32_e32 v2, s26, v2
	v_cmp_lt_u32_e64 s1, 47, v3
	s_or_b32 s33, s1, s33
	s_andn2_b32 exec_lo, exec_lo, s33
	s_cbranch_execz .LBB0_2
.LBB0_48:                               ;   Parent Loop BB0_3 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	v_add_nc_u32_e32 v0, s3, v3
	v_mov_b32_e32 v1, 0
	v_cmp_gt_u32_e64 s1, s15, v0
	v_mov_b32_e32 v0, 0
	s_and_b32 s1, s34, s1
	s_and_saveexec_b32 s35, s1
	s_cbranch_execz .LBB0_47
; %bb.49:                               ;   in Loop: Header=BB0_48 Depth=2
	v_add_nc_u32_e32 v90, s27, v3
	v_lshlrev_b64 v[0:1], 3, v[90:91]
	v_add_co_u32 v0, s1, s10, v0
	v_add_co_ci_u32_e64 v1, s1, s11, v1, s1
	s_waitcnt_vscnt null, 0x0
	global_load_dwordx2 v[0:1], v[0:1], off
	s_branch .LBB0_47
.LBB0_50:                               ; %Flow595
	v_add_nc_u32_e32 v3, s2, v92
	v_add_nc_u32_e32 v0, s3, v93
	v_mul_lo_u32 v4, s15, v3
	v_cmp_gt_u32_e64 s2, s14, v3
	v_cmp_gt_u32_e32 vcc_lo, s15, v0
	s_and_b32 s0, s2, vcc_lo
	s_and_saveexec_b32 s1, s0
	s_cbranch_execz .LBB0_52
; %bb.51:
	v_add_nc_u32_e32 v1, v4, v0
	v_mov_b32_e32 v2, 0
	v_lshlrev_b64 v[1:2], 3, v[1:2]
	v_add_co_u32 v1, s0, s12, v1
	v_add_co_ci_u32_e64 v2, s0, s13, v2, s0
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[1:2], v[88:89], off
.LBB0_52:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s1
	v_add_nc_u32_e32 v1, 1, v0
	v_cmp_gt_u32_e64 s0, s15, v1
	s_and_b32 s1, s2, s0
	s_and_saveexec_b32 s3, s1
	s_cbranch_execz .LBB0_54
; %bb.53:
	v_add_nc_u32_e32 v5, v4, v1
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s1, s12, v5
	v_add_co_ci_u32_e64 v6, s1, s13, v6, s1
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[86:87], off
.LBB0_54:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s3
	v_add_nc_u32_e32 v2, 2, v0
	v_cmp_gt_u32_e64 s1, s15, v2
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execz .LBB0_56
; %bb.55:
	v_add_nc_u32_e32 v5, v4, v2
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s2, s12, v5
	v_add_co_ci_u32_e64 v6, s2, s13, v6, s2
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[84:85], off
.LBB0_56:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s3
	v_or_b32_e32 v5, 1, v3
	v_add_nc_u32_e32 v4, s15, v4
	v_cmp_gt_u32_e64 s2, s14, v5
	s_and_b32 s3, s2, vcc_lo
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_84
; %bb.57:
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_85
.LBB0_58:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execz .LBB0_60
.LBB0_59:
	v_add_nc_u32_e32 v5, v4, v2
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s2, s12, v5
	v_add_co_ci_u32_e64 v6, s2, s13, v6, s2
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[78:79], off
.LBB0_60:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s3
	v_or_b32_e32 v5, 2, v3
	v_add_nc_u32_e32 v4, s15, v4
	v_cmp_gt_u32_e64 s2, s14, v5
	s_and_b32 s3, s2, vcc_lo
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_86
; %bb.61:
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_87
.LBB0_62:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execz .LBB0_64
.LBB0_63:
	v_add_nc_u32_e32 v5, v4, v2
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s2, s12, v5
	v_add_co_ci_u32_e64 v6, s2, s13, v6, s2
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[72:73], off
.LBB0_64:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s3
	v_or_b32_e32 v5, 3, v3
	v_add_nc_u32_e32 v4, s15, v4
	v_cmp_gt_u32_e64 s2, s14, v5
	s_and_b32 s3, s2, vcc_lo
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_88
; %bb.65:
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_89
.LBB0_66:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execz .LBB0_68
.LBB0_67:
	v_add_nc_u32_e32 v5, v4, v2
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s2, s12, v5
	v_add_co_ci_u32_e64 v6, s2, s13, v6, s2
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[66:67], off
.LBB0_68:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s3
	v_or_b32_e32 v5, 4, v3
	v_add_nc_u32_e32 v4, s15, v4
	v_cmp_gt_u32_e64 s2, s14, v5
	s_and_b32 s3, s2, vcc_lo
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_90
; %bb.69:
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_91
.LBB0_70:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execz .LBB0_72
.LBB0_71:
	v_add_nc_u32_e32 v5, v4, v2
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s2, s12, v5
	v_add_co_ci_u32_e64 v6, s2, s13, v6, s2
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[60:61], off
.LBB0_72:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s3
	v_or_b32_e32 v5, 5, v3
	v_add_nc_u32_e32 v4, s15, v4
	v_cmp_gt_u32_e64 s2, s14, v5
	s_and_b32 s3, s2, vcc_lo
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_92
; %bb.73:
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_93
.LBB0_74:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execz .LBB0_76
.LBB0_75:
	v_add_nc_u32_e32 v5, v4, v2
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s2, s12, v5
	v_add_co_ci_u32_e64 v6, s2, s13, v6, s2
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[54:55], off
.LBB0_76:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s3
	v_or_b32_e32 v5, 6, v3
	v_add_nc_u32_e32 v4, s15, v4
	v_cmp_gt_u32_e64 s2, s14, v5
	s_and_b32 s3, s2, vcc_lo
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_94
; %bb.77:
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execnz .LBB0_95
.LBB0_78:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execz .LBB0_80
.LBB0_79:
	v_add_nc_u32_e32 v5, v4, v2
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s2, s12, v5
	v_add_co_ci_u32_e64 v6, s2, s13, v6, s2
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[48:49], off
.LBB0_80:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s3
	v_or_b32_e32 v3, 7, v3
	v_cmp_gt_u32_e64 s2, s14, v3
	v_add_nc_u32_e32 v3, s15, v4
	s_and_b32 s4, s2, vcc_lo
	s_and_saveexec_b32 s3, s4
	s_cbranch_execnz .LBB0_96
; %bb.81:
	s_or_b32 exec_lo, exec_lo, s3
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s0, s3
	s_cbranch_execnz .LBB0_97
.LBB0_82:
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s0
	s_and_b32 s0, s2, s1
	s_and_saveexec_b32 s1, s0
	s_cbranch_execnz .LBB0_98
.LBB0_83:
	s_endpgm
.LBB0_84:
	v_add_nc_u32_e32 v5, v4, v0
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s3, s12, v5
	v_add_co_ci_u32_e64 v6, s3, s13, v6, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[82:83], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execz .LBB0_58
.LBB0_85:
	v_add_nc_u32_e32 v5, v4, v1
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s3, s12, v5
	v_add_co_ci_u32_e64 v6, s3, s13, v6, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[80:81], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execnz .LBB0_59
	s_branch .LBB0_60
.LBB0_86:
	v_add_nc_u32_e32 v5, v4, v0
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s3, s12, v5
	v_add_co_ci_u32_e64 v6, s3, s13, v6, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[76:77], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execz .LBB0_62
.LBB0_87:
	v_add_nc_u32_e32 v5, v4, v1
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s3, s12, v5
	v_add_co_ci_u32_e64 v6, s3, s13, v6, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[74:75], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execnz .LBB0_63
	s_branch .LBB0_64
.LBB0_88:
	v_add_nc_u32_e32 v5, v4, v0
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s3, s12, v5
	v_add_co_ci_u32_e64 v6, s3, s13, v6, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[70:71], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execz .LBB0_66
.LBB0_89:
	v_add_nc_u32_e32 v5, v4, v1
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s3, s12, v5
	v_add_co_ci_u32_e64 v6, s3, s13, v6, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[68:69], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execnz .LBB0_67
	s_branch .LBB0_68
.LBB0_90:
	v_add_nc_u32_e32 v5, v4, v0
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s3, s12, v5
	v_add_co_ci_u32_e64 v6, s3, s13, v6, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[64:65], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execz .LBB0_70
.LBB0_91:
	v_add_nc_u32_e32 v5, v4, v1
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s3, s12, v5
	v_add_co_ci_u32_e64 v6, s3, s13, v6, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[62:63], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execnz .LBB0_71
	s_branch .LBB0_72
.LBB0_92:
	v_add_nc_u32_e32 v5, v4, v0
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s3, s12, v5
	v_add_co_ci_u32_e64 v6, s3, s13, v6, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[58:59], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execz .LBB0_74
.LBB0_93:
	v_add_nc_u32_e32 v5, v4, v1
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s3, s12, v5
	v_add_co_ci_u32_e64 v6, s3, s13, v6, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[56:57], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execnz .LBB0_75
	s_branch .LBB0_76
.LBB0_94:
	v_add_nc_u32_e32 v5, v4, v0
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s3, s12, v5
	v_add_co_ci_u32_e64 v6, s3, s13, v6, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[52:53], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s4, s3
	s_cbranch_execz .LBB0_78
.LBB0_95:
	v_add_nc_u32_e32 v5, v4, v1
	v_mov_b32_e32 v6, 0
	v_lshlrev_b64 v[5:6], 3, v[5:6]
	v_add_co_u32 v5, s3, s12, v5
	v_add_co_ci_u32_e64 v6, s3, s13, v6, s3
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[5:6], v[50:51], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s4
	s_and_b32 s2, s2, s1
	s_and_saveexec_b32 s3, s2
	s_cbranch_execnz .LBB0_79
	s_branch .LBB0_80
.LBB0_96:
	v_add_nc_u32_e32 v4, v3, v0
	v_mov_b32_e32 v5, 0
	v_lshlrev_b64 v[4:5], 3, v[4:5]
	v_add_co_u32 v4, vcc_lo, s12, v4
	v_add_co_ci_u32_e32 v5, vcc_lo, s13, v5, vcc_lo
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[4:5], v[46:47], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s3
	s_and_b32 s3, s2, s0
	s_and_saveexec_b32 s0, s3
	s_cbranch_execz .LBB0_82
.LBB0_97:
	v_add_nc_u32_e32 v0, v3, v1
	v_mov_b32_e32 v1, 0
	v_lshlrev_b64 v[0:1], 3, v[0:1]
	v_add_co_u32 v0, vcc_lo, s12, v0
	v_add_co_ci_u32_e32 v1, vcc_lo, s13, v1, vcc_lo
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[0:1], v[44:45], off
	s_waitcnt_depctr 0xffe3
	s_or_b32 exec_lo, exec_lo, s0
	s_and_b32 s0, s2, s1
	s_and_saveexec_b32 s1, s0
	s_cbranch_execz .LBB0_83
.LBB0_98:
	v_add_nc_u32_e32 v0, v3, v2
	v_mov_b32_e32 v1, 0
	v_lshlrev_b64 v[0:1], 3, v[0:1]
	v_add_co_u32 v0, vcc_lo, s12, v0
	v_add_co_ci_u32_e32 v1, vcc_lo, s13, v1, vcc_lo
	s_waitcnt_vscnt null, 0x0
	global_store_dwordx2 v[0:1], v[42:43], off
	s_endpgm
	.section	.rodata,"a",@progbits
	.p2align	6, 0x0
	.amdhsa_kernel mul
		.amdhsa_group_segment_fixed_size 2688
		.amdhsa_private_segment_fixed_size 0
		.amdhsa_kernarg_size 296
		.amdhsa_user_sgpr_count 6
		.amdhsa_user_sgpr_private_segment_buffer 1
		.amdhsa_user_sgpr_dispatch_ptr 0
		.amdhsa_user_sgpr_queue_ptr 0
		.amdhsa_user_sgpr_kernarg_segment_ptr 1
		.amdhsa_user_sgpr_dispatch_id 0
		.amdhsa_user_sgpr_flat_scratch_init 0
		.amdhsa_user_sgpr_private_segment_size 0
		.amdhsa_wavefront_size32 1
		.amdhsa_uses_dynamic_stack 0
		.amdhsa_system_sgpr_private_segment_wavefront_offset 0
		.amdhsa_system_sgpr_workgroup_id_x 1
		.amdhsa_system_sgpr_workgroup_id_y 1
		.amdhsa_system_sgpr_workgroup_id_z 0
		.amdhsa_system_sgpr_workgroup_info 0
		.amdhsa_system_vgpr_workitem_id 1
		.amdhsa_next_free_vgpr 107
		.amdhsa_next_free_sgpr 41
		.amdhsa_reserve_vcc 1
		.amdhsa_reserve_flat_scratch 0
		.amdhsa_reserve_xnack_mask 0
		.amdhsa_float_round_mode_32 0
		.amdhsa_float_round_mode_16_64 0
		.amdhsa_float_denorm_mode_32 3
		.amdhsa_float_denorm_mode_16_64 3
		.amdhsa_dx10_clamp 1
		.amdhsa_ieee_mode 1
		.amdhsa_fp16_overflow 0
		.amdhsa_workgroup_processor_mode 1
		.amdhsa_memory_ordered 1
		.amdhsa_forward_progress 0
		.amdhsa_shared_vgpr_count 0
		.amdhsa_exception_fp_ieee_invalid_op 0
		.amdhsa_exception_fp_denorm_src 0
		.amdhsa_exception_fp_ieee_div_zero 0
		.amdhsa_exception_fp_ieee_overflow 0
		.amdhsa_exception_fp_ieee_underflow 0
		.amdhsa_exception_fp_ieee_inexact 0
		.amdhsa_exception_int_div_zero 0
	.end_amdhsa_kernel
	.text
.Lfunc_end0:
	.size	mul, .Lfunc_end0-mul
                                        ; -- End function
	.section	.AMDGPU.csdata,"",@progbits
; Kernel info:
; codeLenInByte = 6268
; NumSgprs: 43
; NumVgprs: 107
; ScratchSize: 0
; MemoryBound: 0
; FloatMode: 240
; IeeeMode: 1
; LDSByteSize: 2688 bytes/workgroup (compile time only)
; SGPRBlocks: 5
; VGPRBlocks: 13
; NumSGPRsForWavesPerEU: 43
; NumVGPRsForWavesPerEU: 107
; Occupancy: 9
; WaveLimiterHint : 0
; COMPUTE_PGM_RSRC2:SCRATCH_EN: 0
; COMPUTE_PGM_RSRC2:USER_SGPR: 6
; COMPUTE_PGM_RSRC2:TRAP_HANDLER: 0
; COMPUTE_PGM_RSRC2:TGID_X_EN: 1
; COMPUTE_PGM_RSRC2:TGID_Y_EN: 1
; COMPUTE_PGM_RSRC2:TGID_Z_EN: 0
; COMPUTE_PGM_RSRC2:TIDIG_COMP_CNT: 1
	.text
	.p2alignl 6, 3214868480
	.fill 48, 4, 3214868480
	.ident	"clang version 20.0.0git (git@github.com:Compute-Mirrors/llvm-project 9dfb54abb25a32d9adf38f58b9a78d922f17b167)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.amdgpu_metadata
---
amdhsa.kernels:
  - .args:
      - .address_space:  global
        .name:           srcA
        .offset:         0
        .size:           8
        .type_name:      'MatrixType*'
        .value_kind:     global_buffer
      - .address_space:  global
        .name:           srcB
        .offset:         8
        .size:           8
        .type_name:      'MatrixType*'
        .value_kind:     global_buffer
      - .address_space:  global
        .name:           dstC
        .offset:         16
        .size:           8
        .type_name:      'MatrixType*'
        .value_kind:     global_buffer
      - .name:           dims
        .offset:         24
        .size:           16
        .type_name:      DeviceInfo
        .value_kind:     by_value
      - .offset:         40
        .size:           4
        .value_kind:     hidden_block_count_x
      - .offset:         44
        .size:           4
        .value_kind:     hidden_block_count_y
      - .offset:         48
        .size:           4
        .value_kind:     hidden_block_count_z
      - .offset:         52
        .size:           2
        .value_kind:     hidden_group_size_x
      - .offset:         54
        .size:           2
        .value_kind:     hidden_group_size_y
      - .offset:         56
        .size:           2
        .value_kind:     hidden_group_size_z
      - .offset:         58
        .size:           2
        .value_kind:     hidden_remainder_x
      - .offset:         60
        .size:           2
        .value_kind:     hidden_remainder_y
      - .offset:         62
        .size:           2
        .value_kind:     hidden_remainder_z
      - .offset:         80
        .size:           8
        .value_kind:     hidden_global_offset_x
      - .offset:         88
        .size:           8
        .value_kind:     hidden_global_offset_y
      - .offset:         96
        .size:           8
        .value_kind:     hidden_global_offset_z
      - .offset:         104
        .size:           2
        .value_kind:     hidden_grid_dims
    .group_segment_fixed_size: 2688
    .kernarg_segment_align: 8
    .kernarg_segment_size: 296
    .language:       OpenCL C
    .language_version:
      - 1
      - 2
    .max_flat_workgroup_size: 256
    .name:           mul
    .private_segment_fixed_size: 0
    .sgpr_count:     43
    .sgpr_spill_count: 0
    .symbol:         mul.kd
    .uniform_work_group_size: 1
    .uses_dynamic_stack: false
    .vgpr_count:     107
    .vgpr_spill_count: 0
    .wavefront_size: 32
    .workgroup_processor_mode: 1
amdhsa.target:   'amdgcn-amd-amdhsa--gfx1010:xnack-'
amdhsa.version:
  - 1
  - 2
...

	.end_amdgpu_metadata
