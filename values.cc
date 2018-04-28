#include "lslmini.hh"

// 1 sign + 39 digits + 1 point + 6 decimals
#define FLOAT_AS_STR_MAX_LENGTH (1 + 39 + 1 + 6)
// 1 "<" + 4 floats + 3 ", " separators + 1 ">"
#define QUAT_AS_STR_MAX_LENGTH (1 + FLOAT_AS_STR_MAX_LENGTH*4 + 3*2 + 1)

void LLASTNode::propagate_values() {
   LLASTNode             *node = get_children();
   if ( node != NULL ) {
      /*
         while ( node->get_next() )
         node = node->get_next(); // start with last node

         while ( node )  {
         node->propagate_values();
         node = node->get_prev();
         }
         */
      while ( node ) {
         node->propagate_values();
         node = node->get_next();
      }
   }

   determine_value();
}

void LLASTNode::determine_value() {
   // none
}

void LLScriptDeclaration::determine_value() {
   LLScriptIdentifier *id = (LLScriptIdentifier *) get_child(0);
   LLASTNode *node = get_child(1);
   if ( node == NULL || node->get_node_type() == NODE_NULL ) {
      // assign a default value to the symbol
      LLScriptConstant *value;
      switch(get_child(0)->get_type()->get_itype()) {
         case LST_INTEGER:       value = new LLScriptIntegerConstant(0); break;
         case LST_FLOATINGPOINT: value = new LLScriptFloatConstant(0.0f); break;
         case LST_KEY:           value = new LLScriptKeyConstant(""); break;
         case LST_STRING:        value = new LLScriptStringConstant(""); break;
         case LST_VECTOR:        value = new LLScriptVectorConstant(0.f, 0.f, 0.f); break;
         case LST_QUATERNION:    value = new LLScriptQuaternionConstant(0.f, 0.f, 0.f, 1.f); break;
         case LST_LIST:          value = new LLScriptListConstant((LLScriptSimpleAssignable *)NULL); break;
         default:                fprintf(stderr, "Impossible"); exit(EXIT_FAILURE);
      }
      ((LLScriptIdentifier *)get_child(0))->get_symbol()->set_constant_value(value);
      return;
   }
   DEBUG( LOG_DEBUG_SPAM, NULL, "set %s const to %p\n", id->get_name(), node->get_constant_value() );
   if ( id->get_type()->get_itype() == node->get_type()->get_itype() ) {
      id->get_symbol()->set_constant_value( node->get_constant_value() );
   } else {
      // Handle implicit casts
      LLScriptConstant *value = node->get_constant_value();
      if (!value) return;
      switch( id->get_type()->get_itype() ) {
         case LST_STRING:
            if ( value->get_type()->get_itype() == LST_KEY )
               id->get_symbol()->set_constant_value( new LLScriptStringConstant( ((LLScriptKeyConstant *)value)->get_value() ) );
            break;
         case LST_KEY:
            if ( value->get_type()->get_itype() == LST_STRING )
               id->get_symbol()->set_constant_value( new LLScriptKeyConstant( ((LLScriptStringConstant *)value)->get_value() ) );
            break;
         case LST_FLOATINGPOINT:
            if ( value->get_type()->get_itype() == LST_INTEGER )
               id->get_symbol()->set_constant_value( new LLScriptFloatConstant( (float)((LLScriptIntegerConstant *)value)->get_value() ) );
            break;
         default: break;
      }
   }
}

void LLScriptExpression::determine_value() {
   DEBUG( LOG_DEBUG_SPAM, NULL, "expression.determine_value() op=%d cv=%s st=%d\n", operation, constant_value ? constant_value->get_node_name() : NULL, get_node_sub_type() );
   if ( constant_value != NULL )
      return; // we already have a value

   if ( get_node_sub_type() != NODE_NO_SUB_TYPE && get_node_sub_type() != NODE_LVALUE_EXPRESSION )
      return; // only check normal and lvalue expressions

   if ( operation == 0 ) {
      if ( get_child(1) == NULL )
         constant_value = get_child(0)->get_constant_value();
      else
         constant_value = NULL;
   } else if ( operation == '=' ) {
      constant_value = get_child(1)->get_constant_value();
   } else {

      LLScriptConstant *left  = get_child(0)->get_constant_value();
      LLScriptConstant *right = get_child(1) ? get_child(1)->get_constant_value() : NULL;

      // we need a constant value from the left, and if we have a right side, it MUST have a constant value too
      if ( left && (get_child(1) == NULL || right != NULL) )
         constant_value = left->operation( operation, right, get_lloc() );
      else
         constant_value = NULL;

   }
}

void LLScriptGlobalVariable::determine_value() {
   // ensure the symbol receives the constant value
   if ( get_child(1)->get_node_type() == NODE_SIMPLE_ASSIGNABLE ) {
      // Handle automatic type casts
      LLScriptIdentifier *id = (LLScriptIdentifier *)get_child(0);
      LLScriptSimpleAssignable *node = (LLScriptSimpleAssignable *)get_child(1);

      if ( id->get_type()->get_itype() == node->get_type()->get_itype() ) {
         id->get_symbol()->set_constant_value( node->get_constant_value() );
      } else {
         // Handle implicit casts
         LLScriptConstant *value = node->get_constant_value();
         if (!value) return;
         switch( id->get_type()->get_itype() ) {
            case LST_STRING:
               if ( value->get_type()->get_itype() == LST_KEY )
                  id->get_symbol()->set_constant_value( new LLScriptStringConstant( ((LLScriptKeyConstant *)value)->get_value() ) );
               break;
            case LST_KEY:
               if ( value->get_type()->get_itype() == LST_STRING )
                  id->get_symbol()->set_constant_value( new LLScriptKeyConstant( ((LLScriptStringConstant *)value)->get_value() ) );
               break;
            case LST_FLOATINGPOINT:
               if ( value->get_type()->get_itype() == LST_INTEGER )
                  id->get_symbol()->set_constant_value( new LLScriptFloatConstant( (float)((LLScriptIntegerConstant *)value)->get_value() ) );
               break;
            default: break;
         }
      }
   } else {
      // assign a default value to the symbol
      LLScriptConstant *value;
      switch(get_child(0)->get_type()->get_itype()) {
         case LST_INTEGER:       value = new LLScriptIntegerConstant(0); break;
         case LST_FLOATINGPOINT: value = new LLScriptFloatConstant(0.0f); break;
         case LST_KEY:           value = new LLScriptKeyConstant(""); break;
         case LST_STRING:        value = new LLScriptStringConstant(""); break;
         case LST_VECTOR:        value = new LLScriptVectorConstant(0.f, 0.f, 0.f); break;
         case LST_QUATERNION:    value = new LLScriptQuaternionConstant(0.f, 0.f, 0.f, 1.f); break;
         case LST_LIST:          value = new LLScriptListConstant((LLScriptSimpleAssignable *)NULL); break;
         default:                fprintf(stderr, "Impossible"); exit(EXIT_FAILURE);
      }
      ((LLScriptIdentifier *)get_child(0))->get_symbol()->set_constant_value(value);
   }
}

void LLScriptSimpleAssignable::determine_value() {
   if ( get_child(0) ) {
      constant_value = get_child(0)->get_constant_value();
   }
}

void LLScriptVectorConstant::determine_value() {
   if ( get_value() != NULL )
      return;

   LLASTNode                 *node       = get_children();
   float                     v[3];
   int                       cv = 0;

   for ( node = get_children(); node; node = node->get_next() ) {
      // if we have too many children, make sure we don't overflow cv
      if ( cv >= 3 )
         return;

      // all children must be constant
      if ( !node->is_constant() )
         return;

      // all children must be float/int constants - get their val or bail if they're wrong
      switch( node->get_constant_value()->get_type()->get_itype() ) {
         case LST_FLOATINGPOINT:
            v[cv++] = ((LLScriptFloatConstant*)node->get_constant_value())->get_value();
            break;
         case LST_INTEGER:
            v[cv++] = ((LLScriptIntegerConstant*)node->get_constant_value())->get_value();
            break;
         default:
            return;
      }

   }

   if ( cv < 3 )  // not enough children
      return;

   value = new LLVector( v[0], v[1], v[2] );

}

void LLScriptQuaternionConstant::determine_value() {
   if ( get_value() != NULL )
      return;

   LLASTNode                 *node       = get_children();
   float                     v[4];
   int                       cv = 0;

   for ( node = get_children(); node; node = node->get_next() ) {
      // if we have too many children, make sure we don't overflow cv
      if ( cv >= 4 )
         return;

      // all children must be constant
      if ( !node->is_constant() )
         return;

      // all children must be float/int constants - get their val or bail if they're wrong
      switch( node->get_constant_value()->get_type()->get_itype() ) {
         case LST_FLOATINGPOINT:
            v[cv++] = ((LLScriptFloatConstant*)node->get_constant_value())->get_value();
            break;
         case LST_INTEGER:
            v[cv++] = ((LLScriptIntegerConstant*)node->get_constant_value())->get_value();
            break;
         default:
            return;
      }

   }

   if ( cv < 4 ) // not enough children;
      return;

   value = new LLQuaternion( v[0], v[1], v[2], v[3] );

}


void LLScriptIdentifier::determine_value() {
   // can't determine value if we don't have a symbol
   if ( symbol == NULL )
      return;

   DEBUG( LOG_DEBUG_SPAM, NULL, "id %s assigned %d times\n", get_name(), symbol->get_assignments() );
   if ( symbol->get_assignments() == 0 ) {
      constant_value = symbol->get_constant_value();
      if ( constant_value != NULL && member != NULL ) { // getting a member
         switch ( constant_value->get_type()->get_itype() ) {
            case LST_VECTOR: {
                                LLScriptVectorConstant *c = (LLScriptVectorConstant *)constant_value;
                                LLVector *v = (LLVector *) c->get_value();
                                if ( v == NULL ) {
                                   constant_value = NULL;
                                   break;
                                }
                                switch ( member[0] ) {
                                   case 'x': constant_value = new LLScriptFloatConstant( v->x ); break;
                                   case 'y': constant_value = new LLScriptFloatConstant( v->y ); break;
                                   case 'z': constant_value = new LLScriptFloatConstant( v->z ); break;
                                   default:  constant_value = NULL;
                                }
                                break;
                             }
            case LST_QUATERNION: {
                                    LLScriptQuaternionConstant *c = (LLScriptQuaternionConstant *)constant_value;
                                    LLQuaternion *v = (LLQuaternion *) c->get_value();
                                    if ( v == NULL ) {
                                       constant_value = NULL;
                                       break;
                                    }
                                    switch ( member[0] ) {
                                       case 'x': constant_value = new LLScriptFloatConstant( v->x ); break;
                                       case 'y': constant_value = new LLScriptFloatConstant( v->y ); break;
                                       case 'z': constant_value = new LLScriptFloatConstant( v->z ); break;
                                       case 's': constant_value = new LLScriptFloatConstant( v->s ); break;
                                       default:  constant_value = NULL;
                                    }
                                    break;
                                 }
            default: constant_value = NULL; break;
         }
      }
   }
}

void LLScriptListExpression::determine_value() {
   LLASTNode                 *node       = get_children();
   LLScriptSimpleAssignable  *assignable = NULL;

   // if we have children
   if ( node->get_node_type() != NODE_NULL ) {
      // make sure they are all constant
      for ( node = get_children(); node; node = node->get_next() ) {
         if ( !node->is_constant() )
            return;
      }

      // create assignables for them
      for ( node = get_children(); node; node = node->get_next() ) {
         if ( assignable == NULL ) {
            assignable = new LLScriptSimpleAssignable( node->get_constant_value() );
         } else {
            assignable->add_next_sibling( new LLScriptSimpleAssignable(node->get_constant_value()) );
         }
      }
   }

   // create constant value
   constant_value = new LLScriptListConstant( assignable );

}

void LLScriptVectorExpression::determine_value() {
   LLASTNode                 *node       = get_children();
   float                     v[3];
   int                       cv = 0;

   // don't need to figure out a value if we already have one
   if ( constant_value != NULL )
      return;

   for ( node = get_children(); node; node = node->get_next() ) {
      // if we have too many children, make sure we don't overflow cv
      if ( cv >= 3 )
         return;

      // all children must be constant
      if ( !node->is_constant() )
         return;

      // all children must be float/int constants - get their val or bail if they're wrong
      switch( node->get_constant_value()->get_type()->get_itype() ) {
         case LST_FLOATINGPOINT:
            v[cv++] = ((LLScriptFloatConstant*)node->get_constant_value())->get_value();
            break;
         case LST_INTEGER:
            v[cv++] = ((LLScriptIntegerConstant*)node->get_constant_value())->get_value();
            break;
         default:
            return;
      }
   }

   // make sure we had enough children
   if ( cv < 3 )
      return;

   // create constant value
   constant_value = new LLScriptVectorConstant( v[0], v[1], v[2] );

}

// FIXME: duped code
void LLScriptQuaternionExpression::determine_value() {
   LLASTNode                 *node       = get_children();
   float                     v[4];
   int                       cv = 0;

   if ( constant_value != NULL )
      return;

   for ( node = get_children(); node; node = node->get_next() ) {
      // if we have too many children, make sure we don't overflow cv
      if ( cv >= 4 )
         return;

      // all children must be constant
      if ( !node->is_constant() )
         return;

      // all children must be float/int constants - get their val or bail if they're wrong
      switch( node->get_constant_value()->get_type()->get_itype() ) {
         case LST_FLOATINGPOINT:
            v[cv++] = ((LLScriptFloatConstant*)node->get_constant_value())->get_value();
            break;
         case LST_INTEGER:
            v[cv++] = ((LLScriptIntegerConstant*)node->get_constant_value())->get_value();
            break;
         default:
            return;
      }

   }

   if ( cv < 4 )
      return;

   // create constant value
   constant_value = new LLScriptQuaternionConstant( v[0], v[1], v[2], v[3] );

}

static void float_to_str(float f, char *s, int *n, int dp) {
   if (dp > 6) {
      printf("No more than 6 decimal places supported");
      exit(1);
   }
   if (mono_mode && f != 0.0f) {
      if (f == f + f) {
         *n = sprintf(s, "%s", f > 0 ? "Infinity" : "-Infinity");
      } else if (f != f) {
         *n = sprintf(s, "%s", "NaN");
      } else {
         // Mono float to string conversion, 7 significant digits + rounding.
         // Uses 7 extra decimal places to determine rounding in addition to
         // the requested digits.
         char buf[FLOAT_AS_STR_MAX_LENGTH + 7 + 1];
         int len = sprintf(buf, "%.*f", dp + 7, f);
         if (!strncmp(buf, dp == 5 ? "-0.00000" : "-0.000000", dp + 3) && buf[dp + 3] < '5') {
            // Underflown nonzero negative floats return positive zero
            *n = sprintf(s, "%.*f", dp, 0.0f);
         } else {
            int sgn = buf[0] == '-';
            int i = sgn;
            int digits = 0;

            if (sgn) s[0] = '-';

            // Search for first nonzero from the left
            while (buf[i] == '0' || buf[i] == '.') ++i;
            // Count 7 significant digits, or stop after dp digits after the period
            for ( i = sgn; i < len - 7; i++ ) {
               if (digits == 7)
                  break;
               digits += buf[i] != '.';
            }
            // 'i' is now our rounding point. Check if rounding must be applied.
            int roundidx = buf[i + (buf[i] == '.')] < '5' ? 0 : i;

            // Clear all digits starting here
            for ( ; i < len - 7; i++ ) {
               if (buf[i] != '.') buf[i] = '0';
            }
            if (roundidx) {
               // Rounding needed - increment
               for ( i = roundidx - 1; i >= sgn; i-- ) {
                  if (buf[i] != '.') {
                     if (buf[i] == '9') buf[i] = '0'; else { buf[i]++; break; }
                  }
               }
            }
            // i < sgn means the carry propagated all the way to the first
            // digit and we have to expand the number to fit an extra '1'
            s[sgn] = '1'; // store a leading '1' - will be overwritten if no carry
            *n = (i < sgn) + len - 7;
            strncpy(&s[sgn + (i < sgn)], &buf[sgn], len - 7 - sgn);
            s[*n] = 0;
         }
      }
   } else {
      *n = sprintf(s, "%.*f", dp, f);
   }
   if ( *n > FLOAT_AS_STR_MAX_LENGTH ) {
      printf("Oopsie! We've overflown the buffer!");
      abort(); // PANIC!
   }
}

static void get_vq_as_string(LLScriptConstant *value, char *buf, int dp)
{
   char *s = buf;
   int length;
   int n_elements;
   float f[4];
   if ( value->get_type()->get_itype() == LST_VECTOR ) {
      n_elements = 3;
      LLVector *v = ((LLScriptVectorConstant*)value)->get_value();
      f[0] = v->x;
      f[1] = v->y;
      f[2] = v->z;
   } else {
      n_elements = 4;
      LLQuaternion *q = ((LLScriptQuaternionConstant*)value)->get_value();
      f[0] = q->x;
      f[1] = q->y;
      f[2] = q->z;
      f[3] = q->s;
   }
   *s++ = '<';
   for ( int i = 0; i < n_elements; i++ ) {
      if (i) {
         *s++ = ',';
         *s++ = ' ';
      }
      float_to_str(f[i], s, &length, dp);
      s += length;
   }
   *s++ = '>';
   *s = 0;
}

void LLScriptTypecastExpression::determine_value() {
   LLASTNode                 *node       = get_children();
   LLScriptConstant          *value;

   if ( constant_value != NULL || !node->is_constant() )
      return;

   value = node->get_constant_value();
   // Type cast of a type to itself is a NOP
   if ( value->get_type()->get_itype() == type->get_itype() ) {
      constant_value = value;
      return;
   }

   // (list)x generates a list of 1 element for all types except list (handled above)
   if ( type->get_itype() == LST_LIST ) {
      LLScriptSimpleAssignable *element = new LLScriptSimpleAssignable(value);
      constant_value = new LLScriptListConstant(element);
      return;
   }

   // Perform the type cast in the rest of cases
   switch( type->get_itype() ) {
      case LST_KEY:
         switch( value->get_type()->get_itype() ) {
            case LST_STRING:
               constant_value = new LLScriptKeyConstant(((LLScriptStringConstant*)value)->get_value());
               break;
            default:
               break;
         }
         break;
      case LST_STRING:
         switch( value->get_type()->get_itype() ) {
            case LST_KEY:
               constant_value = new LLScriptStringConstant(((LLScriptKeyConstant*)value)->get_value());
               break;
            case LST_INTEGER:
               {
                  char *buf = new char[11];
                  sprintf(buf, "%d", ((LLScriptIntegerConstant*)value)->get_value());
                  constant_value = new LLScriptStringConstant(buf);
               }
               break;
            case LST_FLOATINGPOINT:
               {
                  // max length = 1 minus sign, 39 digits, 1 point, 6 decimals, 1 null
                  char buf[FLOAT_AS_STR_MAX_LENGTH + 1];
                  float f = ((LLScriptFloatConstant*)value)->get_value();
                  int length;
                  float_to_str(f, buf, &length, 6);
                  char *s = new char[length + 1];
                  strcpy(s, buf);
                  constant_value = new LLScriptStringConstant(s);
               }
               break;
            case LST_VECTOR:
            case LST_QUATERNION:
               {
                  char buf[QUAT_AS_STR_MAX_LENGTH + 1];
                  get_vq_as_string(value, buf, 5);
                  char *s = new char[strlen(buf) + 1];
                  strcpy(s, buf);
                  constant_value = new LLScriptStringConstant(s);
               }
               break;
            case LST_LIST:
               {
                  LLScriptSimpleAssignable *element;
                  std::string result;
                  char buf[QUAT_AS_STR_MAX_LENGTH + 1];
                  for ( element = ((LLScriptListConstant*)value)->get_value(); element; element = (LLScriptSimpleAssignable*)element->get_next() ) {
                     LLScriptConstant *lv = element->get_children()->get_constant_value();
                     if ( !lv ) return;
                     switch( lv->get_type()->get_itype() ) {
                        case LST_INTEGER:
                           {
                              sprintf(buf, "%d", ((LLScriptIntegerConstant*)lv)->get_value());
                              result.append(buf);
                           }
                           break;
                        case LST_FLOATINGPOINT:
                           {
                              float f = ((LLScriptFloatConstant*)lv)->get_value();
                              int length;
                              float_to_str(f, buf, &length, 6);
                              result.append(buf);
                           }
                           break;
                        case LST_VECTOR:
                        case LST_QUATERNION:
                           {
                              get_vq_as_string(lv, buf, 6);
                              result.append(buf);
                           }
                           break;
                        case LST_STRING:
                           result.append(((LLScriptStringConstant*)lv)->get_value());
                           break;
                        case LST_KEY:
                           result.append(((LLScriptKeyConstant*)lv)->get_value());
                           break;
                        default:
                           break;
                     }
                  }
                  char *s = new char[result.length() + 1];
                  strcpy(s, result.c_str());
                  constant_value = new LLScriptStringConstant(s);
               }
               break;
            default:
               break;
         }
         break;
      case LST_FLOATINGPOINT:
         switch( value->get_type()->get_itype() ) {
            case LST_INTEGER:
               constant_value = new LLScriptFloatConstant((float)(((LLScriptIntegerConstant*)value)->get_value()));
               break;
            case LST_STRING:
               {
                  double d;
                  float f = 0.0f;
                  sscanf( ((LLScriptStringConstant *)value)->get_value(), "%lf", &d );
                  f = (float)d;
                  if (mono_mode && (d > -1.1754943157898259e-38 && d < 1.1754943157898259e-38))
                     f = 0.0f;
                  constant_value = new LLScriptFloatConstant(f);
               }
               break;
            default:
               break;
         }
         break;
      case LST_INTEGER:
         switch( value->get_type()->get_itype() ) {
            case LST_FLOATINGPOINT:
               {
                  constant_value = new LLScriptIntegerConstant((int32_t)(((LLScriptFloatConstant*)value)->get_value()));
               }
               break;
            case LST_STRING:
               {
                  const char *s = ((LLScriptStringConstant*)value)->get_value();
                  int32_t result = 0;
                  int iresult;
                  unsigned uresult;
                  if ( s[0] == '0' && (s[1] == 'X' || s[1] == 'x') ) {
                     if (strspn(s + 2, "0123456789ABCDEFabcdef") > 8)
                        result = -1;
                     else if (sscanf(s, "%x", &uresult) == 1)
                        result = (int32_t)uresult;
                  } else {
                     // In 64 bits, we need to detect overflow ourselves
                     int len = strlen(s);
                     char *buf = new char[len + 1];
                     char *sgn = new char[len + 1];
                     buf[0] = 0;
                     sgn[0] = 0;
                     if (sscanf(s, " %[+-]%[0-9]", sgn, buf) != 2)
                        sscanf(s, " %[0-9]", buf);
                     int buflen = strlen(buf);
                     int sgnlen = strlen(sgn);
                     if (sgnlen <= 1 && buflen > 10) {
                        result = -1;
                     } else if (sgnlen <= 1 && buflen >= 1) {
                        if (buflen == 10 && strcmp(buf, "4294967296") >= 0) {
                           result = -1;
                        } else if (sscanf(s, "%d", &iresult) == 1) {
                           result = (int32_t)iresult;
                        }
                     }
                     delete buf;
                     delete sgn;
                  }
                  constant_value = new LLScriptIntegerConstant(result);
               }
               break;
            default:
               break;
         }
         break;
      case LST_VECTOR:
      case LST_QUATERNION:
         if ( value->get_type()->get_itype() == LST_STRING ) {
            const char *s = ((LLScriptStringConstant*)value)->get_value();
            if ( type->get_itype() == LST_VECTOR ) {
               float f[3];
               LLScriptVectorConstant *result = new LLScriptVectorConstant(0.f,0.f,0.f);
               if ( sscanf(s, "<%f,%f,%f>", &f[0], &f[1], &f[2]) == 3 ) {
                  result->get_value()->x = f[0];
                  result->get_value()->y = f[1];
                  result->get_value()->z = f[2];
               }
               constant_value = result;
            } else {
               float f[4];
               LLScriptQuaternionConstant *result = new LLScriptQuaternionConstant(0.f,0.f,0.f,1.f);
               if ( sscanf(s, "<%f,%f,%f,%f>", &f[0], &f[1], &f[2], &f[3]) == 4 ) {
                  result->get_value()->x = f[0];
                  result->get_value()->y = f[1];
                  result->get_value()->z = f[2];
                  result->get_value()->s = f[3];
               }
               constant_value = result;
            }
         }
         break;
      default:
         break;
   }
}
