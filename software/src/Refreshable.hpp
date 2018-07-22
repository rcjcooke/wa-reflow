#ifndef __REFRESHABLE_H_INCLUDED__
#define __REFRESHABLE_H_INCLUDED__

/**
 * An interface implemented by objects that can refresh themselves 
 */
class Refreshable {
public:
  virtual ~Refreshable();
  virtual void refresh() = 0;
};

#endif // __REFRESHABLE_H_INCLUDED__
