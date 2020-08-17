
import React from 'react';
import {
  StyleSheet,
  View,
} from 'react-native';

import {Card,
  Title,
  Paragraph,
  Button} from 'react-native-paper';
  import { Icon, Divider } from 'react-native-elements'

import 'intl';
import 'intl/locale-data/jsonp/en';
 
  function timeConversor(time){
    const ye = new Intl.DateTimeFormat('en', { year: 'numeric' }).format(time);
    const mo = new Intl.DateTimeFormat('en', { month: 'short' }).format(time);
    const da = new Intl.DateTimeFormat('en', { day: '2-digit' }).format(time);

    return da + ' ' + mo + ' ' + ye;
  }  

var todo = {}

  const myCard = ({navigation , todoInfo,completeTodo,cancelTodo}) => {
todo = todoInfo
console.log(todo)
    return (
<Card style={[styles.card, {borderColor: 
          todo.origin==="Google Tasks" || 
          todo.origin==="Google Calendar" || 
          todo.origin==="Google Gmail" ? "red" 
          : todo.origin ==="Outlook Calendar" || 
          todo.origin ==="Outlook emails" ? "blue"
          : "black"   }]}>
          <Card.Content style={styles.cardContent}>
            <Title>{todoInfo.name}</Title>
              <Button 
              icon="plus" 
              color="#000000"  
              compact='false' 
              onPress={() => navigation.navigate('Details',{todoInfo:todoInfo,completeTodo:completeTodo,cancelTodo:cancelTodo})}></Button> 
          </Card.Content>

          <Card.Content style={styles.cardContent}>
            <Paragraph style={styles.dateSize}>{todoInfo.origin}</Paragraph>
            {todoInfo.date ? (
                <View style={styles.date}>
                <Icon
      name='date-range' size={10} color="grey" />
                <Paragraph style={styles.dateSize}>{timeConversor(todoInfo.date)}</Paragraph>
              </View>
    
            ):(
              <View></View> 
            )}
                      </Card.Content>
           <Divider></Divider> 
          <Card.Content style={styles.cardContent}>
            <View style={styles.date}>
              <Icon name='check' size={10} color="green" />
              <Paragraph style={styles.comban}>Complete</Paragraph>
            </View>
            <View style={styles.date}>
              <Icon name='block' size={10} color="red" />
              <Paragraph style={styles.comban}>Cancel</Paragraph>
            </View>
            
          </Card.Content>

        </Card>
    )
  }


    const styles = StyleSheet.create({
        card: {
          flex:1,
          marginLeft: 25,
          marginRight:25,
          marginBottom:10,
          marginTop:10,
          borderWidth:2,
          borderColor: todo.origin==="Google Tasks" ? "red" : "blue"       
          
        },
        cardContent: {
          flex :1,
          backgroundColor: "#eaeaea",
          flexDirection:'row',  
          justifyContent:'space-between',
          
          
        },
        date:{
          flexDirection: "row",
          justifyContent: "space-between",
          alignItems: "center"
          
        },
        dateSize: {
          fontSize: 12,
          color:'grey' 
        },
      
        comban:{
          fontSize: 10,
        }
      
      });    

      export default myCard;