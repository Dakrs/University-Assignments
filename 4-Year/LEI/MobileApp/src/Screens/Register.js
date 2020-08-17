import React, { useState } from 'react'
import { StackActions, NavigationActions } from 'react-navigation'
import { StatusBar,
   ActivityIndicator, 
   AsyncStorage ,
   View,
   StyleSheet,
   Text,
   TextInput,
   TouchableOpacity
} from 'react-native'

import api from '../Services/api'

export default function Register(props) {
  const [username, setUsername] = useState('')
  const [password, setPassword] = useState('')
  const [repeatPassword, setRepeatPassword] = useState('')
  const [loading, setLoading] = useState(false)
  const [errorMessage, setErrorMessage] = useState(null)

 
  async function signIn() {
    if (password.length < 8) {
      setErrorMessage("Error! Password to small")
      return}
    if(!validateIsEmail(username)){
      setErrorMessage("Error! email is Invalid")
      return;
    } 
    if(password!==repeatPassword ){
        setErrorMessage("Error! Put the same password")
        return
    }
    setLoading(true)
    try {
        
            const credentials = {
                email: username,
                pwd: password
            }

      const response = await api.post('/register', credentials)   
     
      const resetAction = StackActions.reset({
        index: 0,
        actions: [NavigationActions.navigate({ routeName: 'SignIn' })],
      })

      setLoading(false)

      props.navigation.dispatch(resetAction)
    } catch (err) {
      console.log(err)

      setLoading(false)
      setErrorMessage('User already exist')
    }
  }

const Back = () => {
    const resetAction = StackActions.reset({
        index: 0,
        actions: [NavigationActions.navigate({ routeName: 'SignIn' })],
      })
      props.navigation.dispatch(resetAction)

}
function validateIsEmail(email) {
  return /^\w+([\.-]?\w+)*@\w+([\.-]?\w+)*(\.\w{2,3})+$/.test(email);
}


  return (
    <View style={styles.Container}>
      <StatusBar barStyle="light-content" />

      <Text style={styles.Title}>meTodo</Text>
      <Text style={styles.TextInformation}>
        Your To-do List
      </Text>

      {!!errorMessage && <Text style={styles.Error}>{errorMessage}</Text>}

      <View style={styles.Form}>
        <TextInput
          style={styles.Input}
          autoCapitalize="none"
          autoCorrect={false}
          placeholder="Email"
          underlineColorAndroid="rgba(0, 0, 0, 0)"
          value={username}
          onChangeText={username => setUsername(username)}
        />

        <TextInput
          style={styles.Input}
          autoCapitalize="none"
          autoCorrect={false}
          placeholder="Password"
          underlineColorAndroid="rgba(0, 0, 0, 0)"
          secureTextEntry={true}
          value={password}
          onChangeText={password => setPassword(password)}
        />
        <TextInput
          style={styles.Input}
          autoCapitalize="none"
          autoCorrect={false}
          placeholder="Repeat password"
          underlineColorAndroid="rgba(0, 0, 0, 0)"
          secureTextEntry={true}
          value={repeatPassword}
          onChangeText={repeatPassword => setRepeatPassword(repeatPassword)}
        />

        <TouchableOpacity onPress={signIn} style={styles.Button}>
          {loading ? (
            <ActivityIndicator size="small" color="#FFF" />
          ) : (
            <Text style={styles.ButtonText}>Next</Text>
          )}
        </TouchableOpacity>
        <Text style={styles.TextInformation}>Have an account ?</Text>
        <Text style={styles.LinkText} onPress={Back}>Log in</Text>
      </View>


    </View>
  )
}

Register.navigationOptions = () => {
  return {
    headerShown:false
  }
}


const styles = StyleSheet.create({
  Container: {
    display: "flex",
    flex: 1,
    backgroundColor: "grey",
    padding:40,
    justifyContent:"center",
    alignItems:"stretch"
  },
  Title:{
    textAlign:"center",
    color:"#ffffff",
    fontSize:24,
    fontWeight:"bold"
  },
  TextInformation:{
    textAlign:"center",
    marginTop:10,
    fontSize:14,
    color:"#dddddd",
    lineHeight:21
  },
  Error:{
    color:"#e37a7a",
    textAlign:"center",
    marginTop:10
  },
  Form: {
    marginTop:20
  },
  Input:{
    backgroundColor: "#ffffff",
    borderRadius:3,
    height:44,
    marginBottom:10
  },
  Button:{
    backgroundColor:"#7e91da",
    borderRadius:3,
    height:44,
    marginTop:10,
    justifyContent:"center",
    alignItems:"center",
    
  },
  ButtonText:{
    color:"white",
    fontSize:14,
    fontWeight:"bold"
  },
  LinkText:{
    color:"blue",
    marginTop:10,
    textDecorationLine:"underline",
    textDecorationColor:"blue",
    justifyContent:"center",
    textAlign:"center"
  }
    
});
