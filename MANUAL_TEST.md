# Manual Testing Guide

## Testing Enhanced Fibonacci Heap GUI

### Prerequisites
- Build the project successfully
- Have a display environment (X11 or Wayland) or use offscreen rendering

### Test Case 1: Basic Insert and Visualization
**Steps:**
1. Launch FibonacciHeapGUI
2. Type `10` in the input field
3. Click "Insert"
4. Repeat with values: 5, 15, 3, 20

**Expected Results:**
- Each number appears as a node in the heap
- Nodes are colored light green (root nodes)
- Minimum node (3) is colored gold
- Status bar shows "Inserted: X" after each insertion
- Info bar shows correct heap size and min value

**✓ Pass if:** All nodes visible, colors correct, status updates appear

---

### Test Case 2: Find Min Highlighting
**Steps:**
1. With heap from Test Case 1
2. Click "Find Min (Highlight)" button

**Expected Results:**
- Minimum node changes to bright yellow
- Status bar shows "Minimum value: 3 (highlighted in yellow)"
- Other nodes remain their original colors

**✓ Pass if:** Minimum node is highlighted in yellow

---

### Test Case 3: Node Selection
**Steps:**
1. With heap from Test Case 1
2. Hover mouse over different nodes
3. Click on a node with value 15

**Expected Results:**
- Tooltip appears on hover showing: Key, Value, Degree, Marked status
- Cursor changes to pointing hand when over nodes
- Clicked node gets red selection ring
- Status bar shows "Node selected: key=15, degree=X, marked=No/Yes"

**✓ Pass if:** Node selection visible with red ring, tooltip works

---

### Test Case 4: Extract Min Animation
**Steps:**
1. With heap from Test Case 1
2. Click "Extract Min (Animated)"
3. Observe the animation

**Expected Results:**
- Animation plays showing:
  - Minimum node highlighted
  - Node being removed
  - Consolidation process
- Status bar shows "Extracting min: 3 - Watch the animation!"
- Animation completes and heap reorganizes
- New minimum is now 5

**✓ Pass if:** Animation runs smoothly, heap correctly updated

---

### Test Case 5: Decrease Key with Animation
**Steps:**
1. Insert nodes: 30, 20, 25, 15, 10
2. Click on node with value 30 to select it
3. Set spin box to value `5`
4. Click "Decrease Key (Animated)"

**Expected Results:**
- Animation shows:
  - Node highlighted
  - Key value changes to 5
  - If violation, node is cut from parent
  - Node moves to root list
  - Marked status may change (orange color)
- Status bar shows operation details
- New key is now the minimum (5)

**✓ Pass if:** Animation shows cuts, node correctly repositioned

---

### Test Case 6: Delete Node
**Steps:**
1. With existing heap
2. Click on any node to select it
3. Click "Delete Selected Node"

**Expected Results:**
- Selected node is removed from heap
- Heap reorganizes correctly
- Status bar shows "Deleted node with key X"
- Heap size decreases by 1

**✓ Pass if:** Node removed, heap still valid

---

### Test Case 7: Union Operation
**Steps:**
1. Create heap with values: 10, 20, 30
2. Click "Union with New Heap"

**Expected Results:**
- New nodes (50, 75, 100) appear in the heap
- Status bar shows "Merged with new heap containing 3 nodes"
- Info bar shows increased heap size (6 total)
- All roots visible in root list

**✓ Pass if:** Heap size increases, new nodes visible

---

### Test Case 8: Animation Speed Control
**Steps:**
1. Insert several nodes
2. Set speed slider to 1 (Slow)
3. Click "Extract Min"
4. Observe animation speed
5. Try speeds 2 and 3

**Expected Results:**
- Speed 1: Slow, easy to follow animation
- Speed 2: Normal speed
- Speed 3: Fast animation
- All speeds work correctly

**✓ Pass if:** Speed changes are noticeable

---

### Test Case 9: Pause/Resume Animation
**Steps:**
1. Insert several nodes
2. Click "Extract Min"
3. While animating, click "Pause"
4. Wait a moment
5. Click "Resume"

**Expected Results:**
- Animation pauses when "Pause" clicked
- Button text changes to "Resume"
- Animation continues from same point when resumed
- Button text changes back to "Pause"

**✓ Pass if:** Pause and resume work correctly

---

### Test Case 10: Reset Heap
**Steps:**
1. With any heap containing nodes
2. Click "Reset Heap"

**Expected Results:**
- All nodes disappear
- Canvas shows "Heap is empty - Click Insert to add nodes"
- Info bar shows "Heap Size: 0"
- Status bar shows "Heap reset - Ready to insert new values"
- All selections cleared

**✓ Pass if:** Heap completely cleared

---

### Test Case 11: Visual Pointer Lines
**Steps:**
1. Insert multiple values to create a complex structure
2. Perform Extract Min to create child nodes
3. Observe the visual representation

**Expected Results:**
- Dashed curved lines connect sibling nodes (root list)
- Solid lines with arrows connect parents to children
- Lines are clearly visible and not overlapping
- Node positions prevent overlaps

**✓ Pass if:** Pointer lines visible, layout is clear

---

### Test Case 12: Tooltip Information
**Steps:**
1. Insert nodes: 10, 20, 30
2. Extract min (to mark some nodes)
3. Hover over different nodes

**Expected Results:**
- Tooltip shows:
  - Key: X
  - Value: X
  - Degree: X  
  - Marked: Yes/No
- Information is accurate for each node
- Tooltip appears quickly on hover

**✓ Pass if:** Tooltips show correct information

---

### Test Case 13: Cascading Cuts
**Steps:**
1. Build a heap with parent-child relationships
2. Insert: 50, 40, 30, 20, 10
3. Extract min several times to create structure
4. Select a child node
5. Decrease its key to less than parent
6. Watch for cascading cuts

**Expected Results:**
- Cut happens when heap property violated
- If parent was already marked, cascading cut occurs
- Nodes move to root list
- Orange coloring indicates marked nodes
- Animation shows the cascade effect

**✓ Pass if:** Cascading cuts animate correctly

---

## Summary Checklist

- [ ] Basic insert works
- [ ] Find Min highlights correctly
- [ ] Node selection works
- [ ] Extract Min animates
- [ ] Decrease Key animates with cuts
- [ ] Delete Node works
- [ ] Union merges heaps
- [ ] Animation speed control works
- [ ] Pause/Resume works
- [ ] Reset clears heap
- [ ] Pointer lines visible
- [ ] Tooltips show correct info
- [ ] Cascading cuts animate

## Notes

- All tests should pass for the GUI to be considered fully functional
- Any failures should be documented with details
- Performance should be smooth with up to 50-100 nodes
- Colors should match the legend provided in the UI
