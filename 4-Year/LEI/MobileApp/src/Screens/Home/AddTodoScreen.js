/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 *
 * @format
 * @flow strict-local
 */

import React,{useState, useEffect}  from 'react';
import { View, StyleSheet } from "react-native";
import {Picker} from '@react-native-community/picker'
import { TextInput,Button,Text,Title, Divider } from 'react-native-paper';


const AddTodoScreen = ({navigation,submitNewTodo})=> {

    const [name, setName] = useState('');
    const [selectedValue, setSelectedValue] = useState("3");
    const [description, setDescription] = useState('');

_clear = () => {
    setName("")
    setSelectedValue("3")
    setDescription("")
}

_createTodo = () => {
    console.log(name)
    console.log(selectedValue)
   submitNewTodo(name,selectedValue,description)
}


    return (
        <View style={styles.container} >
            <Title style={{alignItems:'center'}}>New Todo </Title>
            <TextInput
                    label="Name"
                    value={name}
                    mode='outlined'
                    theme={{ colors: { primary: 'grey',underlineColor:'transparent',}}}
                    style={styles.margin}
                    onChangeText={name => setName(name)}
            />
            <Text>Priority</Text>
            <View style={styles.Picker}>
                <Picker
                    selectedValue={selectedValue}
                    onValueChange={(itemValue, itemIndex) => setSelectedValue(itemValue)}
                    
                >
        
                    <Picker.Item label="1" value="1" />
                    <Picker.Item label="2" value="2" />
                    <Picker.Item label="3" value="3" />
                    <Picker.Item label="4" value="4" />
                    <Picker.Item label="5" value="5" />
                </Picker>
            </View>
            <TextInput
                    label="Description"
                    value={description}
                    mode='outlined'
                    multiline={true}
                    theme={{ colors: { primary: 'grey',underlineColor:'transparent',}}}
                    onChangeText={description => setDescription(description)}
                    style={styles.margin}
            />
            <View style={styles.buttons}>
                <Button mode="contained" onPress={() => _createTodo()} style={styles.button} labelStyle={{fontSize:10,color:'black'}}>
                    Add
                </Button>
                <Button mode="contained" onPress={()=>_clear()}  style={styles.button} labelStyle={{fontSize:10,color:'black'}}>
                    clear
                </Button>
            </View>
        </View>

    );
  };

  const styles = StyleSheet.create({
    container: {
      flex: 1,
      justifyContent:'center',
      paddingTop: 40,
      marginLeft: 25,
      marginRight:25, 
     
    },
    margin:{
     marginBottom:10   
    },
    Picker:{
     height: 50, 
     width: 150,
     borderWidth:1,
     marginBottom:10,
     borderColor:'grey'
    },
    button:{
    height: 25, 
    width: 100,
    fontSize:10,
    backgroundColor:'transparent'
    },
    buttons:{
        flex:1,
        flexDirection:'row',
        justifyContent:'space-between'
    }
    
  });

export default AddTodoScreen;
