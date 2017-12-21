/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2013 Blender Foundation.
 * All rights reserved.
 *
 * Original Author: Joshua Leung
 * Contributor(s): None Yet
 *
 * ***** END GPL LICENSE BLOCK *****
 */

/** \file depsgraph/intern/nodes/deg_node.h
 *  \ingroup depsgraph
 */

#pragma once

#include "intern/depsgraph_types.h"

#include "BLI_utildefines.h"

struct ID;
struct GHash;
struct Scene;

namespace DEG {

struct Depsgraph;
struct DepsRelation;
struct OperationDepsNode;

/* *********************************** */
/* Base-Defines for Nodes in Depsgraph */

/* All nodes in Depsgraph are descended from this. */
struct DepsNode {
	/* Helper class for static typeinfo in subclasses. */
	struct TypeInfo {
		TypeInfo(eDepsNode_Type type, const char *tname, int id_recalc_tag = 0);
		eDepsNode_Type type;
		const char *tname;
		int id_recalc_tag;
	};
	/* Relationships between nodes
	 * The reason why all depsgraph nodes are descended from this type (apart
	 * from basic serialization benefits - from the typeinfo) is that we can have
	 * relationships between these nodes!
	 */
	typedef vector<DepsRelation *> Relations;

	/* Identifier - mainly for debugging purposes. */
	const char *name;
	/* Structural type of node. */
	eDepsNode_Type type;
	/* Nodes which this one depends on. */
	Relations inlinks;
	/* Nodes which depend on this one. */
	Relations outlinks;

	/* Generic tags for traversal algorithms. */
	int done;
	int tag;

	/* Methods. */
	DepsNode();
	virtual ~DepsNode();

	virtual string identifier() const;
	string full_identifier() const;

	virtual void init(const ID * /*id*/,
	                  const char * /*subdata*/) {}

	virtual void tag_update(Depsgraph * /*graph*/) {}

	virtual OperationDepsNode *get_entry_operation() { return NULL; }
	virtual OperationDepsNode *get_exit_operation() { return NULL; }

	virtual eDepsNode_Class get_class() const;
};

/* Macros for common static typeinfo. */
#define DEG_DEPSNODE_DECLARE \
	static const DepsNode::TypeInfo typeinfo
#define DEG_DEPSNODE_DEFINE(NodeType, type_, tname_) \
	const DepsNode::TypeInfo NodeType::typeinfo = DepsNode::TypeInfo(type_, tname_)

void deg_register_base_depsnodes();

}  // namespace DEG
