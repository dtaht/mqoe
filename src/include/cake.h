#pragma once
// Copyright 2023 - Michael David Taht
// SPDX-License-Identifier: AGPL-3.0-only

typedef struct {
	u64 sent_bytes[4];
	u64 sent_packets[4]; // 32 but we need to roll over
	u64 dropped_packets[4]; // FIXME it is annoying that acks_dropped counts here
	u64 ecn_marked_packets[4];
	u64 acks_dropped_packets[4];
	u32 backlog_bytes[4];
// we will care about base (in to out for sparse), peak(sampled), and way_collisions (problem)
// we will also care about sparse/bulk/unresponsive
} cake_stats;
