type state('a) =
  | Idle
  | Loading
  | Loaded('a)
  | Failed(string);

type action('a) =
  | Result('a)
  | Fired
  | Error(string);

let component = ReasonReact.reducerComponent("Container");

let make = (uri, token, query, children) => {
  ...component,
  initialState: () => Idle,
  reducer: (action, _state) =>
    switch action {
    | Result(result) => ReasonReact.Update(Loaded(result))
    | Fired => ReasonReact.Update(Loading)
    | Error(error) => ReasonReact.Update(Failed(error))
    },
  render: ({state, reduce}) => {
    let onQuery = () => {
      reduce(() => Fired, ());
      Transport.run(uri, token, query)
      |> Js.Promise.then_(
           (data) => {
             reduce(() => Result(data), ());
             Js.Promise.resolve(state)
           }
         )
      |> ignore
    };
    children[0](state, onQuery)
  }
};