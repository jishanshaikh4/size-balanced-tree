// CREDITS: https://gist.github.com/jacky860226/03c4fb3fe395495307245032f3635ab2

#ifndef SIZE_BALANCED_TREE
#define SIZE_BALANCED_TREE
template<typename T>
class sb_tree{
	private:
		struct node{
			node *ch[2];
			int s;
			T data;
			node(const T&d):s(1),data(d){}
			node():s(0){ch[0]=ch[1]=this;}
		}*nil,*root;
		inline void rotate(node *&a,bool d){
			node *b=a->ch[!d];
			a->ch[!d]=b->ch[d];
			b->ch[d]=a;
			b->s=a->s;
			a->s=a->ch[0]->s+a->ch[1]->s+1;
			a=b;
		}
		void maintain(node *&o,bool d){
			if(o->ch[d]->ch[d]->s>o->ch[!d]->s){
				rotate(o,!d);
			}else if(o->ch[d]->ch[!d]->s>o->ch[!d]->s){
				rotate(o->ch[d],d);
				rotate(o,!d);
			}else return;
			maintain(o->ch[1],1);
			maintain(o->ch[0],0);
			maintain(o,1);
			maintain(o,0);
		}
		void insert(node *&o,const T&data){
			if(!o->s){
				o=new node(data);
				o->ch[0]=o->ch[1]=nil;
			}else{
				++o->s;
				bool d=o->data<data;
				insert(o->ch[d],data);
				maintain(o,d);
			}
		}
		inline void success_erase(node *&o){
			node *t=o;
			o=o->ch[0]->s?o->ch[0]:o->ch[1];
			delete t;
		}
		bool erase(node *&o,const T&data){
			if(!o->s)return 0;
			if(o->data==data){
				if(!o->ch[0]->s||!o->ch[1]->s)success_erase(o);
				else{
					--o->s;
					node **t=&o->ch[1];
					for(;(*t)->ch[0]->s;t=&(*t)->ch[0])(*t)->s--;
					o->data=(*t)->data;
					success_erase(*t);
				}return 1;
			}else if(erase(o->ch[o->data<data],data)){
				--o->s;return 1;
			}else return 0;
		}
		void clear(node *&o){
			if(o->s)clear(o->ch[0]),clear(o->ch[1]),delete o;
		}
	public:
		sb_tree():nil(new node),root(nil){}
		~sb_tree(){clear(root),delete nil;}
		inline void clear(){clear(root),root=nil;}
		inline void insert(const T&data){
			insert(root,data);
		}
		inline bool erase(const T&data){
			return erase(root,data);
		}
		inline bool find(const T&data){
			node *o=root;
			while(o->s&&o->data!=data)o=o->ch[o->data<data];
			return o->s;
		}
		inline int rank(const T&data){
			int cnt=0;
			for(node *o=root;o->s;)
			if(o->data<data)cnt+=o->ch[0]->s+1,o=o->ch[1];
			else o=o->ch[0];
			return cnt;
		}
		inline const T&kth(int k){
			for(node *o=root;;)
			if(k<=o->ch[0]->s)o=o->ch[0];
			else if(k==o->ch[0]->s+1)return o->data;
			else k-=o->ch[0]->s+1,o=o->ch[1];
		}
		inline const T&operator[](int k){
			return kth(k);
		}
		inline const T&preorder(const T&data){
			node *x=root,*y=0;
			while(x->s)
			if(x->data<data)y=x,x=x->ch[1];
			else x=x->ch[0];
			if(y)return y->data;
			return data;
		}
		inline const T&successor(const T&data){
			node *x=root,*y=0;
			while(x->s)
			if(data<x->data)y=x,x=x->ch[0];
			else x=x->ch[1];
			if(y)return y->data;
			return data;
		}
		inline int size(){return root->s;}
};
#endif
