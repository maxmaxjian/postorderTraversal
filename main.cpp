#include <iostream>
#include <memory>
#include <vector>
#include <iterator>
#include <numeric>
#include <queue>
#include <algorithm>

struct TreeNode {
    int val;
    std::shared_ptr<TreeNode> left, right;
    TreeNode(int v, const std::shared_ptr<TreeNode> & lchild = nullptr, const std::shared_ptr<TreeNode> & rchild = nullptr)
        : val(v), left(lchild), right(rchild) {}

    void print() {
        if (left != nullptr || right != nullptr) {
            std::cout << "val = " << val
                      << "\t -> left = " << (left != nullptr ? std::to_string(left->val) : "null") << std::endl
                      << "\t -> right = " << (right != nullptr ? std::to_string(right->val) : "null") << std::endl;
        }
    }
    
    std::ostream & output(std::ostream & os) {
        os << val;
        return os;
    }
};

using pNode = std::shared_ptr<TreeNode>;

std::ostream & operator<<(std::ostream & os, const pNode & p) {
    if (p != nullptr) {
        p->output(os);
    }
    return os;
}

pNode buildTree(const std::vector<int> & nums) {
    pNode root = nullptr;
    if (nums.empty())
        return root;
    if (nums.size() == 1)
        root = std::make_shared<TreeNode>(nums[0]);
    else {
        size_t mid = nums.size()/2;
        root = std::make_shared<TreeNode>(nums[mid]);
        root->left = buildTree(std::vector<int>(nums.begin(), nums.begin()+mid));
        root->right = buildTree(std::vector<int>(nums.begin()+mid+1, nums.end()));
    }
    return root;
}

void printPostorder(const pNode & node) {
    if (node != nullptr) {
        printPostorder(node->left);
        printPostorder(node->right);
        std::cout << node->val << " ";
    }
}

void printInorder(const pNode & node) {
  if (node != nullptr) {
    printInorder(node->left);
    std::cout << node->val << " ";
    printInorder(node->right);
  }
}

void printPreorder(const pNode & node) {
  if (node != nullptr) {
    std::cout << node->val << " ";
    printPreorder(node->left);
    printPreorder(node->right);
  }
}

void printBFS(const pNode & root) {
    std::queue<pNode> qu;
    qu.push(root);
    while (!qu.empty()) {
        auto curr = qu.front();
        qu.pop();
        if (curr != nullptr) {
            curr->print();
            qu.push(curr->left);
            qu.push(curr->right);
        }
    }
}

pNode getParent(const pNode & root, const pNode & node) {
    if (root != nullptr) {
        std::queue<pNode> qu;
        qu.push(root);
        while (!qu.empty()) {
            auto curr = qu.front();
            qu.pop();
            if (curr != nullptr) {
                if (curr->left == node || curr->right == node)
                    return curr;
                else {
                    qu.push(curr->left);
                    qu.push(curr->right);
                }
            }
        }
    }
    return nullptr;
}

class solution {
  public:
    void postorderTraverse(const pNode & root) {
        pNode first = root;
        while (first->left != nullptr || first->right != nullptr) {
            while (first->left != nullptr)
                first = first->left;
            if (first->right != nullptr)
                first = first->right;
        }
        // std::cout << first->val << " ";
        // pNode next = postorderNext(root, first);
        // while (next != nullptr) {
        //     std::cout << next->val << " ";
        //     first = next;
        //     next = postorderNext(root, first);
        // }
	pNode curr = first;
	while (curr != nullptr) {
	  std::cout << curr->val << " ";
	  curr = postorderNext(root, curr);
	}
    }

  void inorderTraverse(const pNode & root) {
    pNode leftmost = root;
    while (leftmost->left != nullptr)
      leftmost = leftmost->left;
    pNode curr = leftmost;
    while (curr != nullptr) {
      std::cout << curr->val << " ";
      curr = inorderNext(root, curr);
    }
  }

  void preorderTraverse(const pNode & root) {
    pNode curr = root;
    while (curr != nullptr) {
      std::cout << curr->val << " ";
      curr = preorderNext(root, curr);
    }
  }

  private:
    pNode postorderNext(const pNode & root, const pNode & curr) {
        pNode next = nullptr;
        if (curr == root)
            return next;
        else {
            pNode parent = getParent(root, curr);
            if (parent != nullptr) {
                if (curr==parent->left) {
                    if (parent->right == nullptr)
                        next = parent;
                    else {
                        next = parent->right;
                        while (next->left != nullptr || next->right != nullptr) {
                            while (next->left != nullptr)
                                next = next->left;
                            if (next->right == nullptr)
                                return next;
                            else
                                next = next->right;
                        }
                    }
                }
                else
                    next = parent;
            }
        }
        return next;
    }

  pNode inorderNext(const pNode & root, const pNode & curr) {
    pNode next = nullptr;
    if (curr->right != nullptr) {
      next = curr->right;
      while (next->left != nullptr)
	next = next->left;
    }
    else {
      pNode parent = getParent(root, curr);
      if (parent != nullptr) {
	if (curr == parent->left)
	  next = parent;
	else {
	  pNode grandparent = getParent(root, parent);
	  while (grandparent != nullptr && parent != grandparent->left) {
	    parent = grandparent;
	    grandparent = getParent(root, parent);
	  }
	  next = grandparent;
	}
      }
    }
    return next;
  }

  pNode preorderNext(const pNode & root, const pNode & curr) {
    pNode next = nullptr;
    if (curr->left != nullptr)
      next = curr->left;
    else if (curr->right != nullptr)
      next = curr->right;
    else {
      pNode parent = getParent(root, curr);
      if (parent != nullptr) {
	if (curr == parent->left) {
	  if (parent->right != nullptr)
	    next = parent->right;
	  else {
	    pNode grandparent = getParent(root, parent);
	    while (grandparent != nullptr && parent != nullptr && parent->right == nullptr && parent == grandparent->left) {
	      parent = grandparent;
	      grandparent = getParent(root, parent);
	    }
	    if (grandparent != nullptr && parent != nullptr && parent == grandparent->left)
	      next = parent->right;
	  }
	}
	else {
	  pNode grandparent = getParent(root, parent);
	  while (grandparent != nullptr && parent == grandparent->right) {
	    parent = grandparent;
	    grandparent = getParent(root, parent);
	  }
	  if (grandparent != nullptr)
	    next = grandparent->right;
	}
      }
    }
    return next;
  }
};


int main() {
    std::vector<int> nums{1,2,3,4,5,6,7};
    pNode root = buildTree(nums);
    root->left->left->right = std::make_shared<TreeNode>(0);
    root->right->right->left = std::make_shared<TreeNode>(8);
    root->right->right->left->left = std::make_shared<TreeNode>(9);
    printBFS(root);

    std::cout << "Postorder traversal:\n";
    std::cout << "Recursive:\n";
    printPostorder(root);
    std::cout << std::endl;

    solution soln;
    std::cout << "Iterative:\n";
    soln.postorderTraverse(root);
    std::cout << std::endl;

    std::cout << "\nInorder traversal:\n";
    std::cout << "Recursive:\n";
    printInorder(root);
    std::cout << std::endl;
    std::cout << "Iterative:\n";
    soln.inorderTraverse(root);
    std::cout << std::endl;

    std::cout << "\nPreorder traversal:\n";
    std::cout << "Recursive:\n";
    printPreorder(root);
    std::cout << std::endl;
    std::cout << "Iterative:\n";
    soln.preorderTraverse(root);
    std::cout << std::endl;
}
